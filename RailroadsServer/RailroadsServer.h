#ifndef RAILROADSSERVER_H
#define RAILROADSSERVER_H

#include <future>
#include <boost/algorithm/string.hpp>
#include <QObject>
#include <chrono>
#include <QTcpSocket>
#include <QTcpServer>
#include <set>
#include <QtConcurrent/QtConcurrentRun>
#include <QMutex>
#include <QThread>
#include <vector>
#include <string>

#include "rail.h"
#include "StringQueue.h"
#include "trainposindicator.h"
#include "RailroadsCanvas.h"
#include "RailroadsServer.h"
#include "events.h"
#include "RailsGraph.h"
#include "logging.h"

class TrainThread;

class RailroadsServer : public QObject
{
public:
    RailroadsServer(std::string ip, int port, RailsGraph* graph, RailroadsCanvas* canvas);
    void whenConnected();
    void treatMessage(std::string message);
    bool startListening();
    bool isConnected();
    bool isWaiting();
    string getMessage();
    int putMessage(std::string msgToSend);
    const static int minMessage = 8;
    void start();
    void stop();
    void sendGoToRailMessage(string id, string rail);

    QTcpSocket* client;
private:
    RailroadsCanvas* canvas;
    void newConnection();
    void clientDisconnected();
    void receive();
    void msgTreatmentThread();
    void stopAllTrainThreads();
    void REG(std::vector<std::string> words);
    void POS(std::vector<std::string> words);
    void sendDenyToID(std::string id);
    void sendAllowToID(string id, vector<int> lengths);
    vector<bool> negativePaths(vector<string> path);
    vector<string> pathWithoutNegativeSign(vector<string> path, vector<bool> negative);
    bool allRailsInGraph(vector<string> rails);
    vector<int> lengthsOfPath(vector<string> path);
    std::set<TrainThread*> trainThreads;
    bool registerNewTrain(string id, vector<string> path);
    //void trainThread(string id, StringQueue* trainQueue, vector<string> path, vector<bool> negative, vector<int> lengths);
   // void reserveRail(string rail);
    //void releaseRail(string rail);
    void receiveFromClient();


    QTcpServer* tcpServer;
    QString ipStr;
    QHostAddress addr;

    //flags
    bool waitingFlag;
    bool exitFlag;
    std::string localIP;
    quint16 portNum;
    int port;
    bool connected;
    QMutex m;

    StringQueue messages;
    RailsGraph* graph;
};

class TrainThread : public QThread
{
public:
    TrainThread(string id, StringQueue* trainQueue, vector<string> path,
                vector<bool> negative, vector<int> lengths,
                RailsGraph* graph, RailroadsCanvas* canvas, RailroadsServer* server);
    int actualRail;
    string name;
    TrainPosIndicator* indicator;
    StringQueue* evtQueue;
    float pos;
    bool maximal;
    void stop();
    bool updating();
private:
    void run();
    void reserveRail(string rail);
    void releaseRail(string rail);

    bool exitFlag;

    vector<string> rails;
    vector<int> railsLength;
    vector<bool> negative;
    RailsGraph* graph;
    RailroadsCanvas* canvas;
    RailroadsServer* server;
};

#endif // RAILROADSSERVER_H
