#ifndef RAILSGRAPH_H
#define RAILSGRAPH_H
#include <string>
#include <thread>
#include <mutex>
#include <map>
#include <vector>
#include "rail.h"
#include "semaforo.h"
#include <set>

using namespace std;

class RailsGraph
{
public:
    mutable std::mutex m;
    mutable std::mutex railSetLock;

    RailsGraph(std::string graphFilePath);
    ~RailsGraph();

    Rail* getRail(std::string name);
    std::vector<Rail*> getAdjTo(std::string name);
    bool isAdj(std::string first, std::string second);
    bool railInGraph(std::string r);
    void printAdj();

    std::set<std::string> railSet;
    std::map<std::string, Rail*> rails;
    std::map<std::string, Semaforo*> semaphores;
    std::map<std::string, key_t> semaphoreKeyT;
    std::map<std::string, std::vector<Rail*> > adj;

protected:
    static int nextKeyT;

    static const std::string nodeTag, edgesTag, edgesEndTag;

    void addRail(Rail* rail);
    void addAdj(std::string name, Rail* rail);
};

#endif
