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

