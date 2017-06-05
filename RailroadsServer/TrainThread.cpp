#include "RailroadsServer.h"

TrainThread::TrainThread(string id, StringQueue* trainQueue, vector<string> path,
                         vector<bool> negative, vector<int> lengths,
                         RailsGraph* graph, RailroadsCanvas* canvas, RailroadsServer* server) :
    QThread(), name(id), rails(path), negative(negative), railsLength(lengths)
{
    this->evtQueue = trainQueue;
    this->graph = graph;
    this->canvas = canvas;
    this->server = server;
    exitFlag=false;
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

void TrainThread::reserveRail(string rail){
    log("SERVER", string("Trying to enter critical region ") + rail);
    graph->semaphores[rail]->acquire();
    log("SERVER", string("Entered critical region ") + rail);
}

void TrainThread::releaseRail(string rail){
    log("SERVER", string("Trying to exit critical region ") + rail);
    graph->semaphores[rail]->release();
    log("SERVER", string("Exited critical region ") + rail);
}

void TrainThread::stop(){
    exitFlag = true;
}

bool TrainThread::updating(){
    return !exitFlag;
}

void TrainThread::run()
{
    actualRail = 0;
    indicator = NULL;
    while(updating()){
        if(actualRail == rails.size()){
            actualRail = 0;
        }
        reserveRail(rails[actualRail]);
        server->sendGoToRailMessage(name, rails[actualRail]);
        pos = -1;
        maximal = false;
        while(true){
            string* m = evtQueue->pop();
            if(m != NULL){
                pos = -1;
                maximal = false;
                try{
                    pos = std::stoi(*m);
                }catch(...){
                    if(*m == "MAX"){
                        pos = railsLength[actualRail];
                        maximal = true;
                    }else if(*m == "MIN"){
                        pos = 0.0;
                        maximal = true;
                    }else{
                        error("SERVER", *m + string(" is no valid position."));
                    }
                }
                if(pos != -1){
                    canvas->addTrain(rails[actualRail], pos, name, maximal);
                }
            }
            if(maximal){
                break;
            }
        }
        releaseRail(rails[actualRail]);
        actualRail++;
    }
    exitFlag = true;
}



