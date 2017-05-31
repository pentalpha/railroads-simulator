#include "RailsGraph.h"
#include <fstream>
#include <string>
#include <iostream>
#include "rail.h"

using namespace std;

const std::string RailsGraph::nodeTag="[node]";
const std::string RailsGraph::edgesTag="[edges]";
const std::string RailsGraph::edgesEndTag="[end-edges]";

RailsGraph::~RailsGraph(){
    for(std::pair<string, Rail*> railPair : rails){
        delete railPair.second;
    }
}

RailsGraph::RailsGraph(std::string graphFilePath)
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

                    Rail* rail = new Rail(id, length, x1, y1, x2, y2);
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
}

void RailsGraph::printAdj(){
    for(std::pair<std::string, std::vector<Rail*> > pair : adj){
        std::cout << pair.first << ":";
        for(Rail* rail : pair.second){
            std::cout << " " << rail->name;
        }
        std::cout << std::endl;
    }
}

