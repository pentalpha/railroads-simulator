#include "RailroadsViewer.h"
#include "RailroadsCanvas.h"
#include "RailsGraph.h"

RailroadsViewer::RailroadsViewer(int size, int padding, QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("Railroads Viewer");
    QSize qsize((RailroadsCanvas::gridBase*size)*RailroadsCanvas::gridWidth + padding*4,
               (RailroadsCanvas::gridBase*size)*RailroadsCanvas::gridHeigth + padding*4);
    this->resize(qsize.width(), qsize.height());

    graph = new RailsGraph("/home/pentalpha/main/UFRN/2017.1/Lab. Projeto de S.O./railroads-simulator/min.graph");

    graph->printAdj();

    RailroadsCanvas* SFMLView = new RailroadsCanvas(this, size, padding, graph);
    SFMLView->show();
}

RailroadsViewer::~RailroadsViewer()
{

}
