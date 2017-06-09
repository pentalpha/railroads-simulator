#include "RailroadsViewer.h"
#include "TravelMonitor.h"
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
        TrainIndicatorsTest *testA = new TrainIndicatorsTest("A", 0.03, this);
        TrainIndicatorsTest *testB = new TrainIndicatorsTest("B", 0.02, this);
        testA->start();
        testB->start();
    }

    monitor = new TravelMonitor(this);
    monitor->show();
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
        viewer->canvas->updateTrainPos(rail->name, pos, string("Train ") + rail->name, (pos >= length));
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
