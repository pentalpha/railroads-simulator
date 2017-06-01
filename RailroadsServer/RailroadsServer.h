#ifndef RAILROADSSERVER_H
#define RAILROADSSERVER_H

#include "Server.h"
#include "logging.h"
#include <future>
#include <boost/algorithm/string.hpp>

class RailroadsServer : public Server
{
public:
    RailroadsServer(std::string ip, int port);
    void whenConnected();
private:
    void treatMessage(std::string message);
    void REG(std::vector<std::string> words);
    void POS(std::vector<std::string> words);
};

#endif // RAILROADSSERVER_H
