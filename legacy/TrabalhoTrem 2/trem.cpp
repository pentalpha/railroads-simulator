#include "trem.h"
#include <iostream>

Trem::Trem(int id, int x, int y, int iniX, int iniY, int fimX, int fimY)
{
    this->id = id;
    this->x = x;
    this->y = y;
    this->iniX = iniX;
    this->iniY = iniY;
    this->fimX = fimX;
    this->fimY = fimY;
    velocidade = 10;
    enable = true;
}

Trem::~Trem()
{
    threadTrem.join();
}

void Trem::setVelocidade(int velocidade)
{
    this->velocidade = velocidade;
}

void Trem::setEnable(bool enable)
{
    this->enable = enable;
}

void Trem::start()
{
    threadTrem = std::thread(&Trem::run,this);
}

void Trem::run()
{
    while(true){
        std::cout << "oi\n";
        if (enable)
        {
            if (y <= iniY - ladoQuadrado/2 && x < fimX-ladoQuadrado/2){
                x += velocidade;
                y = iniY - ladoQuadrado/2;
            }
            else if (x >= fimX - ladoQuadrado/2 && y < fimY - ladoQuadrado/2){
                y += velocidade;
                x = fimX - ladoQuadrado/2;
            }
            else if (y >= fimY - ladoQuadrado/2 && x > iniX - ladoQuadrado/2){
                x -= velocidade;
                y = fimY - ladoQuadrado/2;
            }
            else if(x >= iniX - ladoQuadrado/2 && y > iniY - ladoQuadrado/2){
                y -= velocidade;
                x = iniX-ladoQuadrado/2;
            }
            emit updateGUI(id,x,y);
        }
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

