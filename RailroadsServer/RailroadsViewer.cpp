#include "RailroadsViewer.h"
#include "RailroadsCanvas.h"
#include "RailsGraph.h"
#include <chrono>
#include <thread>

RailroadsViewer::RailroadsViewer(int size, int padding, RailsGraph* graph, bool test, QWidget *parent)
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
        std::thread testThread(&RailroadsViewer::testTrainIndicatorsA, this);
        testThread.detach();
        std::thread testThreadB(&RailroadsViewer::testTrainIndicatorsB, this);
        testThreadB.detach();
    }
}

void RailroadsViewer::testTrainIndicatorsA(){
    Rail* rail = graph->rails["A"];
    int length = 1;
    float pos = 0.0;
    float speed = 0.03;
    TrainPosIndicator* trainIndicator = NULL;
    while(true){
        if(trainIndicator != NULL){
            trainIndicator->Disown();
        }
        trainIndicator = canvas->addTrain(rail->name, pos, "Train A", (pos >= length));
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        //std::cout << pos << std::endl;
        if(pos >= length){
            break;
        }else{
            pos += speed;
        }
    }
    if(trainIndicator != NULL){
        trainIndicator->Disown();
    }
    std::cout << "Finished TestA Thread" << std::endl;
}

void RailroadsViewer::testTrainIndicatorsB(){
    Rail* rail = graph->rails["B"];
    int length = 1;
    float pos = 0.0;
    float speed = 0.01;
    TrainPosIndicator* trainIndicator = NULL;
    while(true){
        if(trainIndicator != NULL){
            trainIndicator->Disown();
        }
        trainIndicator = canvas->addTrain(rail->name, pos, "Train B", (pos >= length));
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        //std::cout << pos << std::endl;
        if(pos >= length){
            break;
        }else{
            pos += speed;
        }
    }
    if(trainIndicator != NULL){
        trainIndicator->Disown();
    }
    std::cout << "Finished TestB Thread" << std::endl;
}

RailroadsViewer::~RailroadsViewer()
{
    delete canvas;
    delete graph;
}
