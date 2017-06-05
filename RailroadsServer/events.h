#ifndef EVENTS_H
#define EVENTS_H

#include <set>
#include <map>
#include <StringQueue.h>
#include <string>
#include <QMutexLocker>

class Events {
public:
    Events(){
    }

    static QMutexLocker registerQueueLock;
    static std::map<std::string, StringQueue*> EventQueues;
    static std::set<std::string> registeredTrains;
    static StringQueue* getQueue(std::string key);
    static StringQueue* registerQueue(std::string key);
};

#endif // EVENTS_H
