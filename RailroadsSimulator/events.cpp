#include "events.h"

using namespace std;

QMutex Events::registerQueueLock;
std::map<std::string, StringQueue*> Events::EventQueues;
std::set<std::string> Events::registeredTrains;

StringQueue* Events::getQueue(std::string key){
    if(registeredTrains.count(key) > 0){
        registerQueueLock.lock();
        StringQueue* q = EventQueues[key];
        registerQueueLock.unlock();
        return q;
    }
    return NULL;
}

StringQueue* Events::registerQueue(std::string key){
    if(registeredTrains.count(key) == 0){
        registerQueueLock.lock();
        StringQueue *q = new StringQueue;
        registeredTrains.insert(key);
        EventQueues[key] = q;
        registerQueueLock.unlock();

        return q;
    }
    return NULL;
}
