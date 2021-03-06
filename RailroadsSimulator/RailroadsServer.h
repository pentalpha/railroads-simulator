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
#include "trainschedule.h"

#include "RailroadsViewer.h"
#include <QElapsedTimer>
#include <QString>

class TrainThread;

class RailroadsServer : public QObject
{
    Q_OBJECT
public:
    RailroadsServer(std::string ip, int port, RailsGraph* graph, RailroadsCanvas* canvas, RailroadsViewer* viewer);
    bool isConnected();
    bool isWaiting();
    string getMessage();

    bool startListening();
    void start();
    void stop();

    void sendGoToRailMessage(string id, string rail);

    const static int minMessage = 8;
    const static int minBytes = 52;
    QTcpSocket* client;

public slots:
    void putMessage(std::string msgToSend);

signals:
    void messageToSend();

private:
    void newConnection();
    void clientDisconnected();
    void sendAMessage();
    void readAMessage();

    void treatMessage(std::string message);
    void msgTreatmentThread();
    void REG(std::vector<std::string> words);
    void SPD(std::vector<std::string> words);
    void POS(std::vector<std::string> words);
    void sendDenyToID(std::string id);
    void sendAllowToID(string id, vector<int> lengths);
    void changeTrainSpeed(std::string trainId, string newSpeed);
    void registerTrainOnController(string name, float speed);


    void addTrain(TrainSchedule schedule);
    vector<bool> negativePaths(vector<string> path);
    vector<string> pathWithoutNegativeSign(vector<string> path, vector<bool> negative);
    string allRailsInGraph(vector<string> rails);
    vector<int> lengthsOfPath(vector<string> path);
    std::set<TrainThread*> trainThreads;
    bool registerNewTrain(string id, vector<string> path);

    void stopAllTrainThreads();

    QTcpServer* tcpServer;
    QString ipStr;
    QHostAddress addr;
    RailroadsCanvas* canvas;
    //flags
    bool waitingFlag;
    bool exitFlag;
    std::string localIP;
    quint16 portNum;
    int port;
    bool connected;
    QMutex m;

    StringQueue messages, toSend;
    RailsGraph* graph;
    RailroadsViewer* viewer;
};

class TrainThread : public QThread
{
public:
    static const float defaultSpeed;
    TrainThread(string id, StringQueue* trainQueue, vector<string> path,
                vector<bool> negative, vector<int> lengths,
                RailsGraph* graph, RailroadsCanvas* canvas, RailroadsServer* server, RailroadsViewer* viewer,
                float speed = defaultSpeed);
    int actualRail;
    float kmPerSec;
    string name;
    bool off;
    TrainPosIndicator* indicator;
    StringQueue* evtQueue;
    double pos;
    bool maximal;
    void stop();
    bool updating();
private:
    static float valueNotReachingMax(float val, float max, float closeness = 0.1);
    void run();
    void reserveRail(string rail);
    void releaseRail(string rail);
    bool reachedEnd(float pos, float railLength, bool inverse);

    bool exitFlag;

    vector<string> rails;
    vector<int> railsLength;
    vector<bool> negative;
    RailsGraph* graph;
    RailroadsCanvas* canvas;
    RailroadsServer* server;
    RailroadsViewer* viewer;
};

#endif // RAILROADSSERVER_H
