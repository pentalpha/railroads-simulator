from pprint import pprint
from multiprocessing import Queue
import time
import socket
import sys
import threading



'''
constants
'''
HOST = "127.0.0.1"
if(len(sys.argv) >= 2):
	HOST = sys.argv[1]
PORT = 50001
if(len(sys.argv) >= 3):
	PORT = sys.argv[2]
print("Host: " + HOST + "::" + PORT)

startTime = time.perf_counter()

defaultSpeed = 0.25 #km/s

sendLock = threading.Lock()

'''
global data structures
'''
trains = list()
trainSpeed = []
trainStopFlag = dict()
trainDirection = dict()
trainPath = dict()
trainGotoQueues = dict()
toSend = Queue()

'''
functions
'''
#Returns the number of seconds since the program started.
def getUptime():
    return time.perf_counter() - startTime

def sendMsg(msg, server):
    toSend.put(msg)
    #with sendLock:
    #    server.send(msg.encode())

def sendPosUpdate(trainName, pos, server):
    msg = "POS " + trainName + " " + pos + "\n"
    sendMsg(msg, server)

def getRegMsgForTrain(trainName):
    msg = "REG " + trainName
    path = trainPath[trainName]
    negative = trainDirection[trainName]
    for i in range(0,len(path)):
        name = path[i]
        if(negative[i]):
            name = "-" + name
        msg = msg + " " + name
    msg = msg + "\n"
    return msg

def allowTrain(msg):
    #something

def sendTrainTo(msg):
    #something

def denyTrain(msg):
    #something

def treatMsg(msg):
    print("Received " + msg)
    splice = msg.split()
    if '_' in msg:
        splice = msg.split(str='_')
    if 'ALLOW' in msg:
        allowTrain(msg)
    elif 'GOTO' in msg:
        sendTrainTo(msg)
    elif 'DENY' in msg:
        denyTrain(msg)

def finish(sock):
    for train in trains:
        trainStopFlag[train] = True
    tcpSocket.shutdown()
    tcpSocket.close()
    time.sleep(1)

#filePath   path to .json file with train name, path and directions
#return     json representation of the file
def getTrainSchedule(filePath):
    import json
    jsonFile = open(filePath)
    d = json.load(jsonFile)
    return d

def intListToBoolArray(intList):
    boolList = [None]*len(intList)
    #print("boolList has space for " + str(len(boolList)))
    #print("range is: ")
    #print(range(0,len(intList)-1))
    for i in range(0,len(intList)):
        #print(i)
        if(intList[i] > 0):
            boolList[i] = True
        else:
            boolList[i] = False
    return boolList

#fill trains, trainPath and trainDirection from jsonDict
def getTrainDataFromJson(jsonDict):
    for key, val in jsonDict.items():
        trains.append(key)
    for trainName in trains:
        trainPath[trainName] = jsonDict[trainName]["path"]
        #print(trainName + " path has " + str(len(trainPath[trainName])) + " railroads.")
        inverseDirectionArray = intListToBoolArray(jsonDict[trainName]["inverse-direction"])
        #print(trainName + ": ")
        #print(inverseDirectionArray)
        trainDirection[trainName] = inverseDirectionArray

#prepare input data from a .json file
def fillTrainData(filePath):
    jsonDict = getTrainSchedule(filePath)
    getTrainDataFromJson(jsonDict)
    trainSpeed = [None]*len(trains)
    for i in range(0, len(trains)):
        trainSpeed[i] = defaultSpeed
    for train in trains:
        trainGotoQueues[train] = Queue()

def reachedEnd(pos, end, inverse):
    if(inverse):
        return pos <= 0.0
    else:
        return pos >= end

def trainThread(trainName, pathLength, pathDirection, trainGotoQueue, server):
    while(!trainStopFlag[trainName]):
        while(trainGotoQueue.empty()):
            time.sleep(0.1)
        actualRail = trainGotoQueue.get()
        if(actualRail == "STOP"):
            trainStopFlag[trainName] = True
            break
        railLength = pathLength[actualRail]
        inverse = pathDirection[actualRail]
        speed = trainSpeed[trainName]
        pos = 0.0
        if(inverse):
            pos = float(railLength)
        lastUptime = getUptime()
        deltaTime = 0.0
        while(!reachedEnd(pos, railLength, inverse)):
            deltaTime = getUptime() - lastUptime #duration, in seconds, of the last while cycle
            lastUptime = getUptime()
            moved = trainSpeed[trainName]*deltaTime #constant movement rate, in km/s, despite the computer's speed
            if(inverse):
                pos = pos - moved
            else:
                pos = pos + moved
            sendPosUpdate(trainName, str(pos))
        if(inverse):
            sendPosUpdate(trainName, "MIN")
        else:
            sendPosUpdate(trainName, "MAX")

def connectionLoop(tcpSocket):
    inputs = [tcpSocket]
    outputs = [tcpSocket]
    sendQueue = toSend

    while inputs:
        readable, writable, exceptional = select.select(
            inputs, outputs, inputs)
        for s in readable:
            try:
                msg = s.recv(1024)
            except socket.timeout, e:
                err = e.args[0]
                # this next if/else is a bit redundant, but illustrates how the
                # timeout exception is setup
                if err == 'timed out':
                    print ('recv timed out, retry later')
                    sleep(0.01)
                    continue
                else:
                    print('Bad timeout:')
                    print(e)
                    inputs.remove(s)
                    outputs.remove(s)
                    finish(s)
            except socket.error, e:
                print("Something else happened, handle error")
                print(e)
                inputs.remove(s)
                outputs.remove(s)
                finish(s)
            else:
                if len(msg) == 0:
                    print('orderly shutdown on server end')
                    inputs.remove(s)
                    outputs.remove(s)
                    finish(s)
                else:
                    treatMsg(str(msg))
            
        for s in writable:
            try:
                if(!sendQueue.empty()):
                    next_msg = sendQueue.get_nowait()
                    s.send(next_msg.encode())
            except Exception as e:
                print(e)
                inputs.remove(s)
                outputs.remove(s)
                finish(s)

        for s in exceptional:
            inputs.remove(s)
            outputs.remove(s)
            finish(s)

'''
entry point
'''
fillTrainData("../train-schedules/default.json")
for train in trains:
    print(getRegMsgForTrain(train))

tcpSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
dest = (HOST, PORT)
tcpSocket.setblocking(0)
try:
    #connect
    tcpSocket.connect(dest)
    thread = Thread(target = connectionLoop, args = (tcpSocket))
    thread.start()
    
except socket.timeout:
    print("Connection timeout")

finish(tcpSocket)