#include "events.h"

using namespace std;

std::mutex Events::registerQueueLock;
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

        registeredTrains.insert(key);
        StringQueue* stringQueue = new StringQueue;
        EventQueues[key] = stringQueue;

        registerQueueLock.unlock();
        return stringQueue;
    }return NULL;
}
