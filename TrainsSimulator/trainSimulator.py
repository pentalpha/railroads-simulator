from pprint import pprint
from multiprocessing import Queue
import time
import socket
import sys
from threading import Thread, Lock
from blessed import *
import traceback
import select
'''
constants
'''
HOST = "127.0.0.1"
#if(len(sys.argv) >= 2):
#	HOST = sys.argv[1]
PORT = 50002
#if(len(sys.argv) >= 3):
#	PORT = sys.argv[2]

jsonFilePath = "../train-schedules/default.json"
#if(len(sys.argv) >= 4):
#    jsonFilePath = sys.argv[3]

startTime = time.perf_counter()

defaultSpeed = 0.25 #km/s

sendLock = Lock()
onInterface = False
socketFinished = False

'''
global data structures
'''
trains = list()
trainSpeed = dict()
trainStopFlag = dict()
trainDirection = dict()
trainPath = dict()
trainGotoQueues = dict()
toSend = Queue()
trainsWaitingForApproval = dict()
serverLog = list()

'''
functions
'''
#Returns the number of seconds since the program started.
def addToLog(msg):
    serverLog.append(msg)
    print(msg)

def getUptime():
    return time.perf_counter() - startTime

def sendMsg(msg):
    addToLog("Pushed to send queue: " + msg)
    toSend.put(msg)
    #with sendLock:
    #    server.send(msg.encode())

def sendPosUpdate(trainName, pos):
    msg = "POS " + trainName + " " + pos + "\n"
    sendMsg(msg)

def splitMsgBySpaces(msg):
    splice = []
    if ' ' in msg:
        splice = msg.split(' ')
    elif '_' in msg:
        splice = msg.split('_')
    return splice

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

def sendTrainTo(msg):
    splice = splitMsgBySpaces(msg)
    if (not(len(splice) == 3) or not(splice[1] in trains)):
        addToLog("Error, invalid GOTO message: " + msg)
        return
    else:
        id = splice[1]
        railToGo = splice[2]
        trainGotoQueues[id].put(railToGo)

def allowTrain(msg):
    splice = splitMsgBySpaces(msg)
    if len(splice) <= 2:
        addToLog("Error, invalid ALLOW message: " + msg)
    else:
        id = splice[1]
        if not(id in trainsWaitingForApproval):
            addToLog("Train is not waiting for approval: " + msg)
        else:
            trainInfo = trainsWaitingForApproval[id]
            del trainsWaitingForApproval[id]
            if not(len(splice)-2 == len(trainInfo['path'])):
                addToLog("Number of lengths different from number of rails: ")
                addToLog(splice[2:len(splice)])
                addToLog(trainInfo['path'])
            else:    
                lengths = splice[2:len(splice)]
                startTrainThread(trainInfo, lengths)
                addToLog(id + " has been allowed.")

def denyTrain(msg):
    splice = splitMsgBySpaces(msg)
    if(not(len(splice) == 2) or not(splice[1] in trains)):
        addToLog("Error, invalid DENY message: " + msg)
    else:
        id = splice[1]
        del trainsWaitingForApproval[id]
        addToLog(id + " has been denyed.")

def treatMsg(msg):
    decoded = msg
    if decoded.endswith('\n'):
        decoded = decoded[:-1]
    
    if 'ALLOW' in decoded:
        addToLog("Received ALLOW: " + decoded)
        allowTrain(decoded)
    elif 'GOTO' in decoded:
        addToLog("Received GOTO: " + decoded)
        sendTrainTo(decoded)
    elif 'DENY' in decoded:
        addToLog("Received DENY: " + decoded)
        denyTrain(decoded)
    else:
        addToLog("Received Undefined message: " + decoded)

def finish(sock):
    global socketFinished
    if socketFinished:
        addToLog("Already finished socket")
    else:
        addToLog("Finishing socket")
        for train in trains:
            trainStopFlag[train] = True
        sock.shutdown(socket.SHUT_RDWR)
        sock.close()
        socketFinished = True
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
    #addToLog("boolList has space for " + str(len(boolList)))
    #addToLog("range is: ")
    #addToLog(range(0,len(intList)-1))
    for i in range(0,len(intList)):
        #addToLog(i)
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
        #addToLog(trainName + " path has " + str(len(trainPath[trainName])) + " railroads.")
        inverseDirectionArray = intListToBoolArray(jsonDict[trainName]["inverse-direction"])
        #addToLog(trainName + ": ")
        #addToLog(inverseDirectionArray)
        trainDirection[trainName] = inverseDirectionArray

#prepare input data from a .json file
def fillTrainData(filePath):
    jsonDict = getTrainSchedule(filePath)
    getTrainDataFromJson(jsonDict)
    for i in trains:
        trainSpeed[i] = defaultSpeed
    for train in trains:
        trainGotoQueues[train] = Queue()

def regTrains():
    for train in trains:
        trainInfo = dict()
        trainInfo['name'] = train
        trainInfo['path'] = trainPath[train]
        trainsWaitingForApproval[train] = trainInfo
        trainStopFlag[train] = False
        msg = getRegMsgForTrain(train)
        sendMsg(msg)

def reachedEnd(pos, end, inverse):
    if(inverse):
        return pos <= 0.0
    else:
        return pos >= end

def startTrainThread(trainInfo, lengthsArray):
    addToLog("Trying to start thread for " + trainInfo['name'])
    lengths = dict()
    for i in range(0,len(lengthsArray)):
        lengths[trainInfo['path'][i]] = lengthsArray[i]
    trainName = trainInfo['name']
    queue = trainGotoQueues[trainName]
    directionArray = trainDirection[trainName]
    pathDirection = dict()
    for i in range(0,len(directionArray)):
        pathDirection[trainInfo['path'][i]] = directionArray[i]
    thread = Thread(target=trainThread, args=(trainName, lengths, pathDirection, queue))
    addToLog("Starting thread for " + trainInfo['name'])
    #trainStopFlag[trainName] = False
    thread.start()

def trainThread(trainName, pathLength, pathDirection, trainGotoQueue):
    addToLog(trainName + " started in his own thread")
    if trainStopFlag[trainName]:
        addToLog("Tryed to start " + trainName + " but found an exit flag")
        return
    while(trainStopFlag[trainName] == False):
        while(trainGotoQueue.empty()):
            pass
        addToLog(trainName + " finished waiting for GOTO")
        actualRail = trainGotoQueue.get()
        addToLog(trainName + " going to " + actualRail)
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
        addToLog(trainName + " moving through " + actualRail)
        lastPosSent = -3.3333;
        while(not reachedEnd(pos, float(railLength), inverse)):
            deltaTime = getUptime() - lastUptime #duration, in seconds, of the last while cycle
            lastUptime = getUptime()
            moved = trainSpeed[trainName]*deltaTime #constant movement rate, in km/s, despite the computer's speed
            if(inverse):
                pos = pos - moved
            else:
                pos = pos + moved
            if abs(pos - lastPosSent) >= 0.05:
                sendPosUpdate(trainName, str(pos))
                lastPosSent = pos
            #time.sleep(0.1)
        addToLog(trainName + " finished " + actualRail)
        if(inverse):
            sendPosUpdate(trainName, "MIN")
        else:
            sendPosUpdate(trainName, "MAX")
    addToLog("Finishing " + trainName + " thread.")

def connectionLoop(tcpSocket):
    inputs = list()
    inputs.append(tcpSocket)
    outputs = list()
    outputs.append(tcpSocket)
    sendQueue = toSend

    while inputs:
        global socketFinished
        if(socketFinished):
            addToLog("Before select: Socket finished, exiting select loop")
            break
        readable, writable, exceptional = select.select(inputs, outputs, inputs)
        for s in readable:
            try:
                #msg = s.recv(1024)
                msg = s.makefile().readline()
            except socket.timeout as e:
                err = e.args[0]
                # this next if/else is a bit redundant, but illustrates how the
                # timeout exception is setup
                if err == 'timed out':
                    addToLog ('recv timed out, retry later')
                    sleep(0.01)
                    continue
                else:
                    addToLog('Bad timeout:')
                    addToLog(e)
                    finish(s)
            except socket.error as e:
                addToLog("Finishing because: Something else happened, handle error")
                addToLog(e)
                print(traceback.format_exception(None, e, e.__traceback__), file=sys.stderr, flush=True)
                finish(s)
            else:
                if len(msg) == 0:
                    addToLog('Finishing because: orderly shutdown on server end')
                    finish(s)
                else:
                    addToLog("readline: " + msg)
                    treatMsg(msg)

        if(socketFinished):
            addToLog("After readable: Socket finished, exiting select loop")
            break    
        for s in writable:
            try:
                if(not sendQueue.empty()):
                    next_msg = sendQueue.get_nowait()
                    addToLog("Sending: " + next_msg)
                    s.send(next_msg.encode())
            except Exception as e:
                addToLog(e)
                addToLog("Finishing because of exception during msg sending")
                finish(s)

        if(socketFinished):
            addToLog("After writable: Socket finished, exiting select loop")
            break
        for s in exceptional:
            addToLog("Finishing because socket is in exceptional set")
            finish(s)

def display_screen(selection):
    term = Terminal()
    print(term.clear())

    for train in trains:
        text = train
        if train in trainsWaitingForApproval:
            text = text + " (Waiting)"
        text = text + " | " + str(trainSpeed[train]) + "km/s"
        if(selection == 0):
            print('{t.bold_red_reverse}{title}'.format(t=term, title=text))
        else:
            print('{t.normal}{title}'.format(t=term, title=text))
        selection = selection - 1
    print('---- Log with ' + str(len(serverLog)) + ' messages ----')
    for log in reversed(serverLog):
        print('{t.normal}{title}'.format(t=term, title=log))

'''
entry point
'''
fillTrainData(jsonFilePath)
addToLog("Host: " + HOST + "::" + str(PORT))
tcpSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
dest = (HOST, int(PORT))
try:
    #connect
    tcpSocket.connect(dest)
    tcpSocket.setblocking(0)
    thread = Thread(target = connectionLoop, args = (tcpSocket,))
    thread.start()
    regTrains()
    '''
    onInterface = True
    term = Terminal()
    with term.fullscreen():
        selection = 0
        #display_screen(selection)
        selection_inprogress = True
        with term.cbreak():
            while selection_inprogress:
                key = term.inkey()
                if key.is_sequence:
                    if key.name == 'KEY_DOWN':
                        selection += 1
                    if key.name == 'KEY_UP':
                        selection -= 1
                    if key.name == 'KEY_ENTER':
                        selection_inprogress = False
                elif key:
                    addToLog("got {0}.".format(key))

                selection = selection % len(trains)
                #display_screen(selection)
    onInterface = False
    '''
    while not socketFinished:
        pass
except socket.timeout:
    onInterface = False
    addToLog("Connection timeout")
except Exception as e:
    onInterface = False
    addToLog("Unknown error: ")
    addToLog(e)
    print(traceback.format_exception(None, e, e.__traceback__), file=sys.stderr, flush=True)

addToLog("Finishing because reached end of code")
finish(tcpSocket)