#include "trainposindicator.h"

const sf::Color TrainPosIndicator::initialColor = sf::Color(255, 50, 0, 255);

TrainPosIndicator::TrainPosIndicator(sf::Vector2f pos)
{
    orphaned = false;
    dead = false;
    shape = sf::CircleShape(radius, 4);
    shape.setPosition(pos.x - radius/2, pos.y - radius/2);
    shape.setFillColor(initialColor);
}

void TrainPosIndicator::Draw(sf::RenderWindow* window){
    window->draw(shape);
}

void TrainPosIndicator::Disown(){
    this->orphaned = true;
}

bool TrainPosIndicator::isDead(){
    return dead;
}

void TrainPosIndicator::Update(){
    if(!dead){
        if(orphaned){
            sf::Color c = shape.getFillColor();
            if(c.a <= 10 || c.a > 255){
                c.a = 0;
                dead = true;
            }else{
                shape.setFillColor(sf::Color(c.r, c.g, c.b, c.a - 3));
            }
        }
    }
}
