#include "trem.h"

Trem::Trem(int id, int x, int y, int iniX, int iniY, int fimX, int fimY)
{
    this->id = id;
    this->x = x;
    this->y = y;
    this->iniX = iniX;
    this->iniY = iniY;
    this->fimX = fimX;
    this->fimY = fimY;
    velocidade = 250;
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
        switch(id){
        case 1:
            if (enable)
            {
                emit updateGUI(id,x,y);
                if (y == iniY - ladoQuadrado/2 && x < fimX-ladoQuadrado/2)
                    x+=velocidade;
                else if (x >= fimX - ladoQuadrado/2 && y < fimY - ladoQuadrado/2){
                    y+=velocidade;
                    x = fimX;
                }
                else if (x > iniX - ladoQuadrado/2 && y == fimY - ladoQuadrado/2)
                    x-=velocidade;
                else
                    y-=velocidade;
            }
            break;
        case 2:
            //emit updateGUI(id, x,y);
            break;
        default:
            break;
        }
        this_thread::sleep_for(chrono::milliseconds(velocidade));
    }
}

