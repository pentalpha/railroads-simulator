#ifndef RAILROADSCANVAS_H
#define RAILROADSCANVAS_H
#include "QSFMLCanvas.h"
#include "RailsGraph.h"
#include "trainposindicator.h"
#include <iostream>
#include <list>
#include <thread>
#include <mutex>

class RailroadsCanvas : public QSFMLCanvas
{
public:
    RailroadsCanvas(QWidget* parent, int size, int padding, RailsGraph* graph);
    void OnInit();
    void OnUpdate();
    TrainPosIndicator* addTrain(TrainPosIndicator* trainIndicator);
    TrainPosIndicator* addTrain(std::string name, float pos, std::string trainID, bool dark = false);
    ~RailroadsCanvas();

    const static int gridBase = 10;
    const static int gridHeigth = 3;
    const static int gridWidth = 6;

private:
    std::mutex m;
    sf::Vector2f inline pointToPos(int x, int y){
        int multi = sizeMultiplier*gridBase;
        sf::Vector2f point(x*multi + padding,
                           y*multi + padding);
        return point;
    }
    void railShapesFromGraph();
    void removeAllDeadTrains();
    RailsGraph* graph;
    std::vector<sf::RectangleShape> railShapes;
    std::list<TrainPosIndicator*> trainShapes;
    const static int railShapeThickness = 5.f;
    const static sf::Color railShapeColor;
    int sizeMultiplier, padding;
};

#endif // RAILROADSVIEWER_H
