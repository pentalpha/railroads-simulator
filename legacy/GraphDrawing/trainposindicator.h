#ifndef TRAINPOSINDICATOR_H
#define TRAINPOSINDICATOR_H
#include <SFML/Graphics.hpp>

class TrainPosIndicator
{
public:
    TrainPosIndicator(sf::Vector2f pos);
    void Draw(sf::RenderWindow* window);
    void Update();
    void Disown();
    bool isDead();

    bool orphaned;
    bool dead;
    sf::CircleShape shape;
    const static sf::Color initialColor;
    const static int radius = 8;
};

#endif // TRAINPOSINDICATOR_H
