#ifndef _RailroadsViewer_
#define _RailroadsViewer_

#include <QMainWindow>
#include "RailsGraph.h"

class RailroadsViewer : public QMainWindow
{
    Q_OBJECT

public:
    RailroadsViewer(int size, int padding, QWidget *parent = 0);
    ~RailroadsViewer();
    RailsGraph* graph;
};

#endif // MAINWINDOW_H
