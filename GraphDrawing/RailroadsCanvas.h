#ifndef RAILROADSCANVAS_H
#define RAILROADSCANVAS_H
#include "QSFMLCanvas.h"
#include "RailsGraph.h"
#include <iostream>

class RailroadsCanvas : public QSFMLCanvas
{
public:
    RailroadsCanvas(QWidget* parent, int size, int padding, RailsGraph* graph);
    void OnInit();
    void OnUpdate();
    ~RailroadsCanvas();

    const static int gridBase = 10;
    const static int gridHeigth = 3;
    const static int gridWidth = 6;
private:
    sf::Vector2f inline pointToPos(int x, int y){
        int multi = sizeMultiplier*gridBase;
        sf::Vector2f point(x*multi + padding,
                           y*multi + padding);
        return point;
    }
    void railShapesFromGraph();
    RailsGraph* graph;
    std::vector<sf::RectangleShape> railShapes;
    const static int railShapeThickness = 5.f;
    const static sf::Color railShapeColor;
    int sizeMultiplier, padding;
};

#endif // RAILROADSVIEWER_H
