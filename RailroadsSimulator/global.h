#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <string>
#include <qwidget.h>
#include "RailsGraph.h"
#include "RailroadsViewer.h"
#include "RailroadsServer.h"
#include "logging.h"
#include "StringQueue.h"
#include <QSet>
#include <chrono>

using namespace std;

class Global
{
public:
    Global();
    static void startServer(QWidget* parent, int port, string ip, string graphPath);
    static void destroyAll();
    static RailsGraph* graph;
    static RailroadsViewer* viewer;
    static RailroadsServer* server;

};

#endif // GLOBAL_H
