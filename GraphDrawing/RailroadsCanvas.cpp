#include "RailroadsCanvas.h"

const sf::Color RailroadsCanvas::railShapeColor(255, 255, 0, 255);

RailroadsCanvas::RailroadsCanvas(QWidget* parent, int size, int padding, RailsGraph* graph) :
    QSFMLCanvas(parent, QPoint(0, 0),
                QSize(parent->size().width(), parent->size().height()))
{
    this->graph = graph;
    this->padding = padding;
    this->sizeMultiplier = size;
    std::cout << "Width: " << this->size().width() << std::endl;
    std::cout << "Heigth: " << this->size().height() << std::endl;
}

void RailroadsCanvas::OnInit(){
    railShapesFromGraph();
}

void RailroadsCanvas::OnUpdate(){
    //std::cout << "Update screen" << std::endl;
    this->clear(sf::Color(255, 255, 255, 255));
    //int count = 2;
    for(sf::RectangleShape shape : this->railShapes){
        //if(count == 0){
        //    break;
        //}
        this->draw(shape);
        //count--;
    }
    sf::RectangleShape shape(sf::Vector2f(10,10));
    shape.setFillColor(sf::Color::Red);
    for(int i = 0; i <= this->gridWidth; i++){
        for(int j = 0; j <= this->gridHeigth; j++){
            shape.setPosition(this->pointToPos(i, j).x + 5, this->pointToPos(i, j).y + 5);
            this->draw(shape);
        }
    }
}

RailroadsCanvas::~RailroadsCanvas(){

}

void RailroadsCanvas::railShapesFromGraph(){
    for(std::pair<std::string, Rail*> pair : graph->rails){
        int length = pair.second->length * this->gridBase * this->sizeMultiplier;
        sf::Vector2f startingPox = this->pointToPos(pair.second->xStart, pair.second->yStart);
        int rotate;
        if(pair.second->xStart == pair.second->xEnd){
            std::cout << pair.second->name << " is vertical" << std::endl;
            std::cout << startingPox.x << " " << startingPox.y << std::endl;
            rotate = 90;
        }else if (pair.second->yStart == pair.second->yEnd){
            std::cout << pair.second->name << " is horizontal" << std::endl;
            std::cout << startingPox.x << " " << startingPox.y << std::endl;
            rotate = 0;
        }else{
            rotate = 45;
        }
        sf::RectangleShape shape(sf::Vector2f(length, railShapeThickness));
        shape.rotate(rotate);
        shape.setFillColor(railShapeColor);
        shape.setPosition(padding + startingPox.x, padding + startingPox.y);
        this->railShapes.push_back(shape);
    }
}
