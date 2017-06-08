#ifndef TRAINPOSINDICATOR_H
#define TRAINPOSINDICATOR_H
#include <SFML/Graphics.hpp>
#include "logging.h"

struct TrainMove{
    std::string railName;
    float pos;
    std::string trainID;
    bool dark;
};

class TrainPosIndicator
{
public:
    TrainPosIndicator(sf::Vector2f pos, std::string* name, bool dark = false);
    void Draw(sf::RenderWindow* window);
    void UpdatePos(sf::Vector2f newPos, bool dark = false);
    void Update();
    void Disown();
    bool isDead();
    static sf::Font getDroidSans();

    bool orphaned;
    bool dead;
    sf::CircleShape shape;
    const static sf::Color initialColor;
    const static sf::Color darkColor;
    const static int radius = 8;
    static sf::Font droidSans;
    sf::Text trainName;
    float alpha;
};

#endif // TRAINPOSINDICATOR_H
