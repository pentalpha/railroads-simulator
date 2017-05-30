#ifndef _MY_CANVAS_
#define _MY_CANVAS_

#include "QSFMLCanvas.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <QDir>

class MyCanvas : public QSFMLCanvas
{
public :
    MyCanvas(QWidget* Parent, const QPoint& Position, const QSize& Size);
private :
    void OnInit();
    void OnUpdate();
    ~MyCanvas();

    sf::Image  myImage;
    sf::Texture myTexture;
    sf::Sprite mySprite;
    sf::Clock myClock;
};

#endif
