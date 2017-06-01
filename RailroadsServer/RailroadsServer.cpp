#include "RailroadsServer.h"

RailroadsServer::RailroadsServer(std::string ip, int port) :
    Server(ip.c_str(), port)
{
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
}

void RailroadsServer::POS(std::vector<std::string> words){
    log("SERVER-POS", vectorToStr(words));
}
