#ifndef _RailroadsViewer_
#define _RailroadsViewer_

#include <QMainWindow>
#include "RailsGraph.h"
#include "RailroadsCanvas.h"

class RailroadsViewer : public QMainWindow
{
    Q_OBJECT

public:
    RailroadsViewer(int size, int padding, std::string graphFilePath, bool test = false, QWidget *parent = 0);
    void testTrainIndicatorsA();
    void testTrainIndicatorsB();
    ~RailroadsViewer();
    RailsGraph* graph;
    RailroadsCanvas* canvas;
};

#endif // MAINWINDOW_H
