#include "global.h"

RailsGraph* Global::graph = NULL;
RailroadsViewer* Global::viewer = NULL;
Server* Global::server = NULL;
std::mutex Global::m;

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
    server = new Server(ip.c_str(), port);
    server->start();
}

void Global::log(std::string origin, std::string message){
    m.lock();
    std::cout << "[" << origin << "] " << message << std::endl;
    m.unlock();
}

void Global::error(std::string origin, std::string message){
    m.lock();
    std::cerr << "[" << origin << "] " << message << std::endl;
    m.unlock();
}
