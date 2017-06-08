#include "trainposindicator.h"

const sf::Color TrainPosIndicator::initialColor = sf::Color(255, 50, 0, 255);
const sf::Color TrainPosIndicator::darkColor = sf::Color(180, 20, 0, 255);
sf::Font TrainPosIndicator::droidSans = TrainPosIndicator::getDroidSans();

sf::Font TrainPosIndicator::getDroidSans(){
    sf::Font font;
    if(!font.loadFromFile("../DroidSans.ttf")){
        error("TRAIN-INDICATOR", "Could not load droid sans");
    }
    return font;
}

TrainPosIndicator::TrainPosIndicator(sf::Vector2f pos, std::string* name, bool dark)
{
    orphaned = false;
    dead = false;
    shape = sf::CircleShape(radius, 4);
    shape.setPosition(pos.x - radius, pos.y - radius);
    shape.setFillColor(initialColor);
    if(dark){
        shape.setFillColor(darkColor);
    }
    trainName.setFont(droidSans);
    trainName.setString(*name);
    trainName.setColor(sf::Color::Black);
    trainName.setStyle(sf::Text::Bold);
    trainName.setCharacterSize(14);
    trainName.setOrigin(trainName.getLocalBounds().width/2, trainName.getLocalBounds().height/2);
    trainName.setPosition(shape.getPosition().x, shape.getPosition().y - radius*2);
    alpha = 255.0;
}

void TrainPosIndicator::Draw(sf::RenderWindow* window){
    window->draw(shape);
    if(!orphaned && !dead){
        window->draw(trainName);
    }
}

void TrainPosIndicator::Disown(){
    this->orphaned = true;
}

bool TrainPosIndicator::isDead(){
    return dead;
}

void TrainPosIndicator::UpdatePos(sf::Vector2f newPos, bool dark){
    if(dark){
        shape.setFillColor(darkColor);
    }
    shape.setPosition(newPos.x - radius, newPos.y - radius);
    trainName.setPosition(shape.getPosition().x, shape.getPosition().y - radius*2);
}

void TrainPosIndicator::Update(){
    if(!dead){
        if(orphaned){
            sf::Color c = shape.getFillColor();
            if(alpha <= 10){
                c.a = 0;
                alpha = 0;
                dead = true;
            }else{
                alpha -= 0.05;
                shape.setFillColor(sf::Color(c.r, c.g, c.b, alpha));
            }
        }
    }
}
