#ifndef RAILROADSSERVER_H
#define RAILROADSSERVER_H

#include "Server.h"
#include "logging.h"
#include <future>
#include <boost/algorithm/string.hpp>
#include "events.h"
#include "RailsGraph.h"
#include "RailroadsCanvas.h"

class RailroadsServer : public Server
{
public:
    RailroadsServer(std::string ip, int port, RailsGraph* graph, RailroadsCanvas* canvas);
    void whenConnected();
private:
    RailroadsCanvas* canvas;
    void treatMessage(std::string message);
    void REG(std::vector<std::string> words);
    void POS(std::vector<std::string> words);
    void sendDenyToID(std::string id);
    void sendAllowToID(string id, vector<int> lengths);
    vector<bool> negativePaths(vector<string> path);
    vector<string> pathWithoutNegativeSign(vector<string> path, vector<bool> negative);
    bool allRailsInGraph(vector<string> rails);
    vector<int> lengthsOfPath(vector<string> path);
    bool registerNewTrain(string id, vector<string> path);
    void trainThread(string id, StringQueue* trainQueue, vector<string> path, vector<bool> negative, vector<int> lengths);



    RailsGraph* graph;
};

#endif // RAILROADSSERVER_H
