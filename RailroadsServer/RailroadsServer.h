#ifndef RAILROADSSERVER_H
#define RAILROADSSERVER_H

#include "Server.h"

class RailroadsServer : public Server
{
public:
    RailroadsServer(std::string ip, int port);
    void whenConnected();
};

#endif // RAILROADSSERVER_H
