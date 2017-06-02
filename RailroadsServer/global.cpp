#include "global.h"

RailsGraph* Global::graph = NULL;
RailroadsViewer* Global::viewer = NULL;
RailroadsServer* Global::server = NULL;

Global::Global()
{

}

void Global::startServer(QWidget* parent, int port, string ip, string graphPath){
    log("STARTUP", "Creating railroads graph...");
    graph = new RailsGraph(graphPath);
    graph->printAdj();
    int zoom = 10;
    int padding = 40;
    log("STARTUP", "Startint RailroadsViewer...");
    viewer = new RailroadsViewer(zoom, padding, graph, true, parent);
    viewer->show();
    log("STARTUP", "Startint server...");
    server = new RailroadsServer(ip.c_str(), port, graph, viewer->canvas);
}

void Global::destroyAll(){
    if(server->isConnected()){
        server->stop();
    }
    viewer->close();
    delete viewer;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    delete server;
    delete graph;
}
