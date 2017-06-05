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
    deltaClock.restart();
}

void RailroadsCanvas::OnInit(){
    railShapesFromGraph();
    deltaClock.restart();
}


RailroadsCanvas::~RailroadsCanvas(){

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
    //UPDATE
    trainShapes.remove_if(isDead);
    //sf::Time dt = deltaClock.restart();
    indicatorParamsLocker.lock();
    while(indicatorsToMake.size() > 0)
    {
        addTrain(indicatorsToMake.front());
        indicatorsToMake.pop();
    }
    indicatorParamsLocker.unlock();

    for(TrainPosIndicator* train : trainShapes){
        train->Update();
    }
    //DRAW
    this->clear(sf::Color(255, 255, 255, 255));
    for(sf::RectangleShape shape : this->railShapes){
        this->draw(shape);
    }
    drawGridPoints(12);
    for(TrainPosIndicator* train : trainShapes){
        train->Draw(this);
    }
}

void RailroadsCanvas::drawGridPoints(float rsize){
    float rectSize = rsize;
    sf::RectangleShape shape(sf::Vector2f(rectSize,rectSize));
    shape.setFillColor(sf::Color::Red);
    for(int i = 0; i <= this->gridWidth; i++){
        for(int j = 0; j <= this->gridHeigth; j++){
            shape.setPosition(this->pointToPos(i, j).x - rectSize/2,
                              this->pointToPos(i, j).y - rectSize/2);
            this->draw(shape);
        }
    }
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

void RailroadsCanvas::addTrain(std::string name, float pos,
                                             std::string trainID, bool dark){
    TrainPosIndicatorParams indicator;
    indicator.pos = pos;
    indicator.name = name;
    indicator.trainID = trainID;
    indicator.dark = dark;

    indicatorParamsLocker.lock();
    indicatorsToMake.push(indicator);
    indicatorParamsLocker.unlock();
}

void RailroadsCanvas::addTrain(TrainPosIndicatorParams indicator){
    Rail* rail = graph->getRail(indicator.name);
    sf::Vector2f railOrigin = this->pointToPos(rail->xStart, rail->yStart);
    sf::Vector2f trainPos;
    float pos = indicator.pos*sizeMultiplier*gridBase;
    if(rail->yStart == rail->yEnd){
        trainPos = sf::Vector2f(railOrigin.x + pos, railOrigin.y);
    }else if (rail->xStart == rail->xEnd){
        trainPos = sf::Vector2f(railOrigin.x, railOrigin.y + pos);
    }else{
        trainPos = sf::Vector2f(railOrigin.x + pos, railOrigin.y + pos);
    }
    //std::cout << trainPos.x << " " << trainPos.y << std::endl;
    TrainPosIndicator* train = new TrainPosIndicator(trainPos,
                                                     new string(indicator.trainID),
                                                     indicator.dark);
    addTrain(train);
}

void RailroadsCanvas::addTrain(TrainPosIndicator* trainIndicator){
    TrainPosIndicator* oldShape = actualTrainShapes[trainIndicator->trainName.getString()];
    if(oldShape != NULL){
        oldShape->Disown();
    }
    trainShapes.push_back(trainIndicator);
    actualTrainShapes[trainIndicator->trainName.getString()] = trainIndicator;
}
