#include "RailsGraph.h"
#include <fstream>
#include <string>
#include <iostream>
#include "logging.h"
#include "rail.h"

using namespace std;

const std::string RailsGraph::nodeTag="[node]";
const std::string RailsGraph::edgesTag="[edges]";
const std::string RailsGraph::edgesEndTag="[end-edges]";
int RailsGraph::nextKeyT = 1;

RailsGraph::~RailsGraph(){
    for(std::pair<string, Rail*> railPair : rails){
        //delete railPair.second;
    }
    /*for(std::pair<string, QSemaphore*> s : semaphores){
        if(s.second != NULL){
            s.second->release();
            delete s.second;
        }
    }*/
}

RailsGraph::RailsGraph(std::string graphFilePath)
    : m(QMutex::Recursive), railSetLock(QMutex::Recursive),
      rails(), adj(), semaphores()
{
    std::ifstream input(graphFilePath);
    std::string word;
    while(!input.eof()){
        input >> word;
        if(word == nodeTag){
            std::string id, posStart, posEnd;
            int length;
            input >> id;
            input >> length;
            input >> posStart;
            input >> posEnd;
            std::cout << nodeTag << " " << id << " " << length << " " << posStart << " " << posEnd << std::endl;
            //cout << posStart[0] << "|" << posStart[0] << endl;

            if(posStart.length() == 3 && posEnd.length() == 3){
                std::vector<std::string> splitedStart, splitedEnd;
                string strX1(1, posStart[0]);
                string strX2(1, posEnd[0]);
                string strY1(1, posStart[2]);
                string strY2(1, posEnd[2]);
                std::cout << nodeTag << " " << id << " " << length << " "
                          << strX1 << "|"<< strY1 << " "
                          << strX2 << "|"<< strY2 << endl;

                try{
                    int x1, y1, x2, y2;
                    x1 = std::stoi(strX1);
                    y1 = std::stoi(strY1);
                    x2 = std::stoi(strX2);
                    y2 = std::stoi(strY2);


                    if(x1 > x2 || y1 > y2){
                        int auxX, auxY;
                        auxX = x1;
                        auxY = y1;
                        x1 = x2;
                        y1 = y2;
                        x2 = auxX;
                        y2 = auxY;
                    }

                    Rail* rail = new Rail(id, length, x1, y1, x2, y2);
                    if(rail == NULL){
                        error("GRAPH", "Null rail built:");
                        error("GRAPH", id);
                    }
                    this->addRail(rail);
                }catch(...){
                    std::cout << "Error parsing: " << splitedStart[0]  << " " << splitedStart[1]
                              << splitedEnd[0]  << " " << splitedEnd[1] << std::endl;
                }
            }
        }else if(word == edgesTag){
            string idA, idB;
            while(!input.eof()){
                input >> idA;
                if(idA == edgesEndTag){
                    break;
                }
                input >> idB;
                this->addAdj(idA, rails[idB]);
                idA.clear();
                idB.clear();
            }
        }
    }
    //finished
    RailsGraph* graph = this;
    int i = 0;
}

bool RailsGraph::railInGraph(std::string r){
    QMutexLocker locker(&railSetLock);
    bool res = !(railSet.find(r) == railSet.end());
    return res;
}

void RailsGraph::printAdj(){
    QMutexLocker locker(&m);
    for(std::pair<std::string, std::vector<Rail*> > pair : adj){
        std::cout << pair.first << ":";
        for(Rail* rail : pair.second){
            std::cout << " " << rail->name;
        }
        std::cout << std::endl;
    }
}

void RailsGraph::addRail(Rail* rail){
    QMutexLocker locker(&m);
    semaphores[rail->name] = new QSemaphore(1);//new Semaforo(nextKeyT, 1, IPC_CREAT|0600);
    semaphoreKeyT[rail->name] = nextKeyT;
    railSet.insert(rail->name);
    nextKeyT++;
    rails[rail->name] = rail;
}



void RailsGraph::addAdj(std::string name, Rail* rail){
    QMutexLocker locker(&m);
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

Rail* RailsGraph::getRail(std::string name){
    QMutexLocker locker(&m);
    Rail* rail = rails[name];
    if(rail == NULL){
        error("GRAPH",std::string("Requested rail has NULL value: ") + name);
    }
    return rail;
}

std::vector<Rail*> RailsGraph::getAdjTo(std::string name){
    QMutexLocker locker(&m);
    return adj[name];
}

bool RailsGraph::isAdj(std::string first, std::string second){
    QMutexLocker locker(&m);
    for(Rail* rail : getAdjTo(first)){
        if(rail->name == second){
            return true;
        }
    }
    return false;
}

