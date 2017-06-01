#include "logging.h"

std::mutex loggingMutex;

void log(std::string origin, std::string message){
    loggingMutex.lock();
    std::cout << "[" << origin << "] " << message << std::endl;
    loggingMutex.unlock();
}

void error(std::string origin, std::string message){
    loggingMutex.lock();
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

