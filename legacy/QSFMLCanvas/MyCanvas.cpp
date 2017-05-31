#include "MyCanvas.h"

MyCanvas::MyCanvas(QWidget* Parent, const QPoint& Position, const QSize& Size) :
QSFMLCanvas(Parent, Position, Size)
{

}

MyCanvas::~MyCanvas(){

}

void MyCanvas::OnInit()
{
    // Load the image
    std::cout << "onInit" << std::endl;
    if (!myImage.loadFromFile("/home/pentalpha/main/UFRN/2017.1/Lab. Projeto de S.O./railroads-simulator/QSFMLCanvas/sfml.png")) {
        std::cout << "Loading error"<< std::endl;
    } else {
        std::cout << "Image was loaded fine" << std::endl;
    }
    // Setup the sprite
    myTexture.loadFromImage(myImage);
    mySprite.setTexture(myTexture);
    mySprite.setPosition(150, 150);
    std::cout << "setting the texture of the sprite" << std::endl;
    mySprite.setOrigin(sf::Vector2f(myTexture.getSize().x / 2.f, myTexture.getSize().y / 2.f));
    myClock.restart();
}

void MyCanvas::OnUpdate()
{
    RenderWindow::clear(sf::Color(0, 128, 0));
    // Rotate the sprite
    mySprite.rotate(myClock.getElapsedTime().asSeconds() * 100.f);
    // Draw it
    RenderWindow::draw(mySprite);
    myClock.restart();
}
