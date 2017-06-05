#ifndef RAILROADSSERVER_H
#define RAILROADSSERVER_H

#include "logging.h"
#include <future>
#include <boost/algorithm/string.hpp>
#include "events.h"
#include "RailsGraph.h"
#include "RailroadsCanvas.h"
#include <QObject>
#include <chrono>
#include <QTcpSocket>
#include <QTcpServer>
#include <QSet>
#include <QtConcurrent/QtConcurrentRun>
#include <QMutexLocker>
#include "TrainThread.h"

class RailroadsServer : public QObject
{
public:
    RailroadsServer(std::string ip, int port, RailsGraph* graph, RailroadsCanvas* canvas);
    void whenConnected();
    void treatMessage(std::string message);
    const static int minMessage = 8;
    bool start();
    QTcpSocket* client;
private:
    RailroadsCanvas* canvas;

    void REG(std::vector<std::string> words);
    void POS(std::vector<std::string> words);
    void sendDenyToID(std::string id);
    void sendAllowToID(string id, vector<int> lengths);
    vector<bool> negativePaths(vector<string> path);
    vector<string> pathWithoutNegativeSign(vector<string> path, vector<bool> negative);
    bool allRailsInGraph(vector<string> rails);
    vector<int> lengthsOfPath(vector<string> path);
    QSet<TrainThread*> trainThreads;
    bool registerNewTrain(string id, vector<string> path);
    void trainThread(string id, StringQueue* trainQueue, vector<string> path, vector<bool> negative, vector<int> lengths);
    void sendGoToRailMessage(string id, string rail);
    void reserveRail(string rail);
    void releaseRail(string rail);


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
    QMutexLocker m;

    StringQueue messages;
    RailsGraph* graph;
};

#endif // RAILROADSSERVER_H
