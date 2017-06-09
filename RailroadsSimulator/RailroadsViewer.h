#ifndef _RailroadsViewer_
#define _RailroadsViewer_

#include <QMainWindow>
#include <QThread>
#include "RailsGraph.h"
#include "RailroadsCanvas.h"
#include "TravelMonitor.h"

class RailroadsViewer : public QMainWindow
{
    Q_OBJECT

public:
    RailroadsViewer(int size, int padding, RailsGraph* graph, bool test = false, QWidget *parent = 0);
    ~RailroadsViewer();
    RailsGraph* graph;
    TravelMonitor* monitor;
    RailroadsCanvas* canvas;
};

class TrainIndicatorsTest : public QThread{
public:
    TrainIndicatorsTest(string railName , float speed, RailroadsViewer* viewer);
private:
    void run();

    Rail* rail;
    int length;
    float pos;
    float speed;
    TrainPosIndicator* trainIndicator;
    RailroadsViewer* viewer;
};

#endif // MAINWINDOW_H
