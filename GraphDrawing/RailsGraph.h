#ifndef RAILSGRAPH_H
#define RAILSGRAPH_H
#include <string>
#include <thread>
#include <mutex>
#include <map>
#include <vector>
#include "rail.h"

using namespace std;

class RailsGraph
{
public:
    std::map<std::string, Rail*> rails;
    std::map<std::string, std::vector<Rail*> > adj;

protected:
    std::mutex m;
    static const std::string nodeTag, edgesTag, edgesEndTag;

    inline void addRail(Rail* rail){
        m.lock();
        rails[rail->name] = rail;
        m.unlock();
    }

    inline void addAdj(std::string name, Rail* rail){
        if (adj.find(name) == adj.end() ) {
          adj[name] = std::vector<Rail*>();
        }
        adj[name].push_back(rail);
        std::string nameB = rail->name;
        Rail* railA = rails[name];
        if(railA != NULL){
            adj[nameB].push_back(railA);
        }
        //addAdj(rail->name, rails[name]);
    }
public:
    RailsGraph(std::string graphFilePath);

    inline Rail* getRail(std::string name){
        return rails[name];
    }

    inline std::vector<Rail*> getAdjTo(std::string name){
        return adj[name];
    }

    inline bool isAdj(std::string first, std::string second){
        for(Rail* rail : getAdjTo(first)){
            if(rail->name == second){
                return true;
            }
        }
        return false;
    }
    void printAdj();

};

#endif // RAILSGRAPH_H
