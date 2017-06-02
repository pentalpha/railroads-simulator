#include "RailroadsServer.h"

using namespace std;

RailroadsServer::RailroadsServer(std::string ip, int port, RailsGraph* graph, RailroadsCanvas* canvas) :
    Server(ip.c_str(), port)
{
    canvas = canvas;
    graph = graph;
    start();
    std::thread* throwWhenConnectedThread = new std::thread(&RailroadsServer::whenConnected, this);
    startWaiting(throwWhenConnectedThread);
}

void RailroadsServer::whenConnected(){
    log("SERVER", "RailroadsServer is ON");

    while(!exitFlag){
        std::string msg = getMessage();
        if(msg.length() >= minMessage){
            //auto treater = std::thread(&RailroadsServer::treatMessage, this, msg);
            //treater.detach();
            treatMessage(msg);
        }else if (msg.length() >= 2){
            log("[SERVER]", std::string("Message is too short: \n") + msg);
        }
    }

    log("SERVER", "RailroadsServer is OFF");
}

void RailroadsServer::treatMessage(std::string message){
    //log("SERVER", std::string("Treating '") + message + std::string("'"));
    //int posPOS = message.find_first_of("POS");
    std::vector<std::string> words;
    boost::split(words, message, boost::is_any_of("_"));
    if(words.size() > 0){
        std::string first = words[0];
        words.erase(words.begin());
        std::vector<std::string> params = words;
        if(first=="POS"){
            POS(params);
        }else if(first=="REG"){
            REG(params);
        }else{
            log("SERVER", std::string("Message is not a POS or REG: ") + message);
        }
    }else{
        log("SERVER", std::string("No words in message: ") + message);
    }
}

void RailroadsServer::REG(std::vector<std::string> words){
    log("SERVER-REG", vectorToStr(words));
    string id = words[0];
    words.erase(words.begin());
    registerNewTrain(id, words);
}

void RailroadsServer::POS(std::vector<std::string> words){
    log("SERVER-POS", vectorToStr(words));
    string id = words[0];
    words.erase(words.begin());
    if(Events::registeredTrains.count(id) >= 1){
        Events::getQueue(id)->push(new string(words[1].c_str()));
    }
}

void RailroadsServer::sendDenyToID(string id){
    log("SERVER", string("DENYING TO ") + id);
    putMessage(string("DENY_")+id+string("\n"));
}

void RailroadsServer::sendAllowToID(string id, vector<int> lengths){
    log("SERVER", string("Allowing ") + id);
    putMessage(string("ALLOW_")+ id + string("_") + vectorToStr(lengths) + string("\n"));
}

void RailroadsServer::sendGoToRailMessage(string id, string rail){
    string message = "GOTO_";
    message += id;
    message += "_";
    message += rail;
    message += "\n";
    log("SERVER", message);
    putMessage(message);
}

vector<bool> RailroadsServer::negativePaths(vector<string> path){
    vector<bool> negative;
    for(int i = 0; i < path.size(); i++){
        string name = path[i];
        bool found = false;
        if(name.length() >= 2){
            if(name[0] == '-'){
                found = true;
            }
        }
        negative.push_back(found);
    }
    return negative;
}

vector<string> RailroadsServer::pathWithoutNegativeSign(vector<string> path, vector<bool> negative){
    vector<string> noNegative;
    for(int i = 0; i < path.size(); i++){
        string name = path[i];
        if(negative[i]){
            name = name.substr(1);
        }
        noNegative.push_back(name);
    }
    return noNegative;
}

bool RailroadsServer::allRailsInGraph(vector<string> rails){
    for(auto p : rails){
        if (graph->rails.find(p) == graph->rails.end()){
            return false;
        }
    }
    return true;
}

vector<int> RailroadsServer::lengthsOfPath(vector<string> path){
    vector<int> lengths;
    for(int i = 0; i < path.size(); i++){
        lengths.push_back(graph->getRail(path[i])->length);
    }
    return lengths;
}

bool RailroadsServer::registerNewTrain(string id, vector<string> path){
    StringQueue* q = Events::registerQueue(id);
    if(q == NULL){
        sendDenyToID(id);
        return false;
    }
    auto negative = negativePaths(path);
    auto noNegativeSign = pathWithoutNegativeSign(path, negative);
    if(!allRailsInGraph(noNegativeSign)){
        error("SERVER", string("Some of the rails in the given path could not be found")
              + vectorToStr(path));
        sendDenyToID(id);
        return false;
    }
    auto lengths = lengthsOfPath(noNegativeSign);
    auto newTrainThread = thread(&RailroadsServer::trainThread, this, id, q, noNegativeSign,
                                 negative, lengths);
    sendAllowToID(id, lengths);
    newTrainThread.detach();
    return true;
}

void RailroadsServer::trainThread(string id, StringQueue* trainQueue, vector<string> path,
                                  vector<bool> negative, vector<int> lengths){
    int actualRail = 0;
    while(!exitFlag){
        if(actualRail == path.size()){
            actualRail = 0;
        }
        reserveRail(path[actualRail]);
        sendGoToRailMessage(id, path[actualRail]);
        float pos = -1;
        bool maximal = false;
        while(true){
            string* m = trainQueue->pop();
            if(m != NULL){
                pos = -1;
                maximal = false;
                try{
                    pos = std::stoi(*m);
                }catch(...){
                    if(*m == "MAX"){
                        pos = lengths[actualRail];
                        maximal = true;
                    }else if(*m == "MIN"){
                        pos = 0.0;
                        maximal = true;
                    }else{
                        error("SERVER", *m + string(" is no valid position."));
                    }
                }
                if(pos != -1){
                    canvas->addTrain(path[actualRail], pos, id, maximal);
                }
            }
            if(maximal){
                break;
            }
        }
        releaseRail(path[actualRail]);
        actualRail++;
    }
}

/* Estratégia anti deadlock de 3
 * Antes de requisitar entrar no proximo trilho (T1), verificar:
 *  O prox dps (T2) dele está ocupado por um trem (A)?
 *      Não -> Requisitar o proximo (T1)
 *      Sim -> O prox trilho de A (A1) está ocupado?
 *          Não -> Requisitar T1
 *          Sim -> O trem que está em A1 (B) precisa ir para T1?
 *              Não -> Requisitar T1
 *              Sim -> Não requisitar, esperar um pouco, checar novamente
 *
 * Dados: Disponibilizar, para todos os trems, o caminho dos outros trems e a posição atual.
 */

void RailroadsServer::reserveRail(string rail){

}

void RailroadsServer::releaseRail(string rail){

}



