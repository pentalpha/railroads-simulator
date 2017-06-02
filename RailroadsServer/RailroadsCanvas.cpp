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

bool isDead(TrainPosIndicator* train) {
    bool dead = train->isDead();
    if(dead){
        delete train;
        return true;
    }else{
        return false;
    }
}

void RailroadsCanvas::OnUpdate(){
    trainShapes.remove_if(isDead);
    for(TrainPosIndicator* train : trainShapes){
        train->Update();
    }
    //DRAW
    this->clear(sf::Color(255, 255, 255, 255));
    for(sf::RectangleShape shape : this->railShapes){
        this->draw(shape);
    }
    float rectSize = 12;
    sf::RectangleShape shape(sf::Vector2f(rectSize,rectSize));
    shape.setFillColor(sf::Color::Red);
    for(int i = 0; i <= this->gridWidth; i++){
        for(int j = 0; j <= this->gridHeigth; j++){
            shape.setPosition(this->pointToPos(i, j).x - rectSize/2, this->pointToPos(i, j).y - rectSize/2);
            this->draw(shape);
        }
    }
    for(TrainPosIndicator* train : trainShapes){
        train->Draw(this);
    }
}

RailroadsCanvas::~RailroadsCanvas(){

}

void RailroadsCanvas::railShapesFromGraph(){
    for(std::pair<std::string, Rail*> pair : graph->rails){
        int length = pair.second->length * this->gridBase * this->sizeMultiplier;
        sf::Vector2f startingPos = this->pointToPos(pair.second->xStart, pair.second->yStart);
        int rotate;
        if(pair.second->xStart == pair.second->xEnd){
            std::cout << pair.second->name << " is vertical" << std::endl;
            std::cout << startingPos.x << " " << startingPos.y << std::endl;
            rotate = 90;
        }else if (pair.second->yStart == pair.second->yEnd){
            std::cout << pair.second->name << " is horizontal" << std::endl;
            std::cout << startingPos.x << " " << startingPos.y << std::endl;
            rotate = 0;
        }else{
            rotate = 45;
        }
        sf::RectangleShape shape(sf::Vector2f(length, railShapeThickness));
        shape.rotate(rotate);
        shape.setFillColor(railShapeColor);
        if(rotate == 0){
            startingPos = sf::Vector2f(startingPos.x, startingPos.y - railShapeThickness/2);
        }else if(rotate == 90){
            startingPos = sf::Vector2f(startingPos.x + railShapeThickness/2, startingPos.y);
        }
        shape.setPosition(startingPos.x, startingPos.y);
        this->railShapes.push_back(shape);
    }
}

TrainPosIndicator* RailroadsCanvas::addTrain(std::string name, float pos, std::string trainID, bool dark){
    Rail* rail = graph->getRail(name);
    sf::Vector2f railOrigin = this->pointToPos(rail->xStart, rail->yStart);
    sf::Vector2f trainPos;
    pos = pos*sizeMultiplier*gridBase;
    if(rail->yStart == rail->yEnd){
        trainPos = sf::Vector2f(railOrigin.x + pos, railOrigin.y);
    }else if (rail->xStart == rail->xEnd){
        trainPos = sf::Vector2f(railOrigin.x, railOrigin.y + pos);
    }else{
        trainPos = sf::Vector2f(railOrigin.x + pos, railOrigin.y + pos);
    }
    //std::cout << trainPos.x << " " << trainPos.y << std::endl;
    TrainPosIndicator* train = new TrainPosIndicator(trainPos, new string(trainID), dark);
    return addTrain(train);
}

TrainPosIndicator* RailroadsCanvas::addTrain(TrainPosIndicator* trainIndicator){
    m.lock();
    trainShapes.push_back(trainIndicator);
    m.unlock();
    return trainIndicator;
}
