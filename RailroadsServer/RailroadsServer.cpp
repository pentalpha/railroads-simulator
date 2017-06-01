#include "RailroadsServer.h"

RailroadsServer::RailroadsServer(std::string ip, int port) :
    Server(ip.c_str(), port)
{
    start();
    std::thread* throwWhenConnectedThread = new std::thread(&RailroadsServer::whenConnected, this);
    startWaiting(throwWhenConnectedThread);
}

void RailroadsServer::whenConnected(){

}
