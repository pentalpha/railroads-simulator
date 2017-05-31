#include "RailroadsServer.h"

RailroadsServer::RailroadsServer(string ip, int port) :
    Server(ip.c_str(), port)
{
    start();
    startWaiting();
}
