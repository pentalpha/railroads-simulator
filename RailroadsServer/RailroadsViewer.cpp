#include "RailroadsViewer.h"
#include "RailroadsCanvas.h"
#include "RailsGraph.h"
#include <chrono>
#include <thread>

RailroadsViewer::RailroadsViewer(int size, int padding, RailsGraph* graph,
                                 bool test, QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("Railroads Viewer");
    QSize qsize((RailroadsCanvas::gridBase*size)*RailroadsCanvas::gridWidth + padding*2,
               (RailroadsCanvas::gridBase*size)*RailroadsCanvas::gridHeigth + padding*2);
    this->resize(qsize.width(), qsize.height());

    this->graph = graph;

    canvas = new RailroadsCanvas(this, size, padding, graph);
    canvas->show();
    if(test){
        TrainIndicatorsTest testA("A", 0.02, this);
        TrainIndicatorsTest testB("A", 0.02, this);
        testA.start();
        testB.start();
    }
}

TrainIndicatorsTest::TrainIndicatorsTest(string railName, float speed,
                                         RailroadsViewer* viewer)
    : QThread()
{
    this->rail = viewer->graph->getRail(railName);
    this->length = rail->length;
    this->pos = 0.0;
    this->speed = speed;
    this->trainIndicator = NULL;
    this->viewer = viewer;
}

void TrainIndicatorsTest::run(){
    while(true){
        viewer->canvas->addTrain(rail->name, pos, string("Train ") + rail->name, (pos >= length));
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        //std::cout << pos << std::endl;
        if(pos >= length){
            break;
        }else{
            pos += speed;
        }
    }
    std::cout << "Finished TestA Thread" << std::endl;
}

RailroadsViewer::~RailroadsViewer()
{
    delete canvas;
    delete graph;
}
