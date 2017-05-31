#ifndef GLOBAL_H
#define GLOBAL_H

class Server;

#include <iostream>
#include <string>
#include <qwidget.h>
#include <thread>
#include <mutex>
#include "RailsGraph.h"
#include "RailroadsViewer.h"
#include "Server.h"

using namespace std;

class Global
{
public:
    Global();
    static void startServer(QWidget* parent, int port, string ip, string graphPath);
    static void log(std::string origin, std::string message);
    static void error(std::string origin, std::string message);

    static RailsGraph* graph;
    static RailroadsViewer* viewer;
    static Server* server;
    static std::mutex m;
};

#endif // GLOBAL_H
