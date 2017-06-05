#include "logging.h"
#include <QMutexLocker>

QMutexLocker loggingMutex;

void log(std::string origin, std::string message){
    loggingMutex.relock();
    std::cout << "[" << origin << "] " << message << std::endl;
    loggingMutex.unlock();
}

void error(std::string origin, std::string message){
    loggingMutex.relock();
    std::cerr << "[" << origin << "-ERROR] " << message << std::endl;
    loggingMutex.unlock();
}

std::string vectorToStr(std::vector<std::string> words){
    std::string str = "";
    for(auto x : words){
        str += std::string(" ") + x;
    }
    return str;
}

std::string vectorToStr(std::vector<int> v){
    std::string str = "";
    bool first = true;
    for(int x : v){
        if(first){
            first = false;
        }else{
            str += std::string("_");
        }
        str += std::to_string(x);
    }
    return str;
}

