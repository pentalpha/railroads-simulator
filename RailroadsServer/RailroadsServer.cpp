#include "RailroadsServer.h"

RailroadsServer::RailroadsServer(std::string ip, int port) :
    Server(ip.c_str(), port)
{
    start();
    startWaiting();
}
