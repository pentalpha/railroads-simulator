from pprint import pprint

trains = list()
trainDirection = dict()
trainPath = dict()

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


fillTrainData("../train-schedules/default.json")
for train in trains:
    print(getRegMsgForTrain(train))
