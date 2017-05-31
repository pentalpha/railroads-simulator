#include "global.h"
#include "RailsGraph.h"
#include "RailroadsViewer.h"

Global::Global()
{

}

void Global::startServer(QWidget* parent, int port, string ip, string graphPath){
    std::cout << "[Startint server...]" << std::endl;
    RailsGraph* graph = new RailsGraph(graphPath);
    graph->printAdj();
    int zoom = 10;
    int padding = 40;
    RailroadsViewer* viewer = new RailroadsViewer(zoom, padding, graph, true, parent);
    viewer->show();
}
