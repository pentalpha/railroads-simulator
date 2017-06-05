#ifndef TRAINTHREAD_H
#define TRAINTHREAD_H

#include <QThread>
#include "rail.h"
#include "RailsGraph.h"
#include "StringQueue.h"
#include <vector>
#include <string>
#include "trainposindicator.h"
#include "RailroadsCanvas.h"
#include "RailroadsServer.h"

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

    bool exitFlag();

    vector<string> rails;
    vector<int> railsLength;
    vector<bool> negative;
    RailsGraph* graph;
    RailroadsCanvas* canvas;
    RailroadsServer* server;
};

#endif // TRAINTHREAD_H
