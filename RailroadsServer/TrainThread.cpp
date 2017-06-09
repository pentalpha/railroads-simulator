#include "RailroadsServer.h"

const float TrainThread::defaultSpeed = 0.5;

TrainThread::TrainThread(string id, StringQueue* trainQueue, vector<string> path,
                         vector<bool> negative, vector<int> lengths,
                         RailsGraph* graph, RailroadsCanvas* canvas, RailroadsServer* server,
                         RailroadsViewer* viewer, float speed) :
    QThread(), name(id), rails(path), negative(negative), railsLength(lengths)
{
    this->evtQueue = trainQueue;
    this->graph = graph;
    this->canvas = canvas;
    this->server = server;
    this->kmPerSec = speed;
    this->viewer = viewer;
    exitFlag=false;
    this->off = false;
}

float TrainThread::valueNotReachingMax(float val, float max, float closeness){
    if(val < closeness){
        return closeness;
    }else if(val > max-closeness){
        return max-closeness;
    }else{
        return val;
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

void TrainThread::reserveRail(string rail){
    //log("SERVER", string("Trying to enter critical region ") + rail);
    graph->semaphores[rail]->acquire();
    //log("SERVER", string("Entered critical region ") + rail);
}

void TrainThread::releaseRail(string rail){
    //log("SERVER", string("Trying to exit critical region ") + rail);
    graph->semaphores[rail]->release();
    //log("SERVER", string("Exited critical region ") + rail);
}

void TrainThread::stop(){
    exitFlag = true;
}

bool TrainThread::updating(){
    return !exitFlag;
}

bool TrainThread::reachedEnd(float pos, float railLength, bool inverse){
    if(!inverse){
        return pos >= railLength;
    }else{
        return pos <= 0.0;
    }
}

void TrainThread::run()
{
    actualRail = 0;
    bool starting = true;
    QElapsedTimer travelTimer;
    travelTimer.start();
    while(updating()){
        if(actualRail == rails.size()){
            actualRail = 0;
            float secs = (float)(travelTimer.elapsed())/1000.0;
            viewer->monitor->putTravelTime(name, secs);
            travelTimer.restart();
        }
        string railName = rails[actualRail];
        reserveRail(railName);
        float railLength = railsLength[actualRail];
        bool inverse = negative[actualRail];
        if(!starting){
            if(inverse){
                pos = railLength;
            }else{
                pos = 0.0;
            }
        }else{
            pos = railLength/2;
            starting = false;
        }
        QElapsedTimer timer;
        timer.start();
        while(!reachedEnd(pos, railLength, inverse)){
            QString m(evtQueue->pop().c_str());
            if(m == ""){
                float milliseconds = timer.elapsed();
                timer.restart();
                if(!off){
                    float moved = (milliseconds/1000.f) * kmPerSec;
                    if(inverse){
                        pos -= moved;
                    }else{
                        pos += moved;
                    }
                    canvas->updateTrainPos(railName, valueNotReachingMax(pos, railLength),
                                           name, false);
                    //sleep 10ms/1cs
                    QThread::msleep(10);
                }
            }else{
                if(m == QString("STOP")){
                    off = true;
                }else if(m == QString("PLAY")){
                    off = false;
                }else{
                    try{
                        float newSpeed = m.toDouble();
                        //speed change:
                        kmPerSec = newSpeed;
                    }catch(...){
                        error("SERVER", m.toStdString() + string(" is no valid command to a train."));
                    }
                }
            }
        }
        //log("TRAIN",name + string(" has finished rail ") + railName);
        canvas->updateTrainPos(railName, valueNotReachingMax(pos, railLength), name, true);
        releaseRail(rails[actualRail]);
        actualRail++;
    }
    exitFlag = true;
}



