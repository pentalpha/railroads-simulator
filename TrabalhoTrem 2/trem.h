#ifndef TREM_H
#define TREM_H

#include <QObject>
#include <thread>
#include <chrono>
using namespace std;

class Trem : public QObject
{
    Q_OBJECT
public:
    Trem(int,int,int, int, int, int, int);
    ~Trem();
    void start();
    void run();
    void setVelocidade(int);
    void setEnable(bool);

signals:
    void updateGUI(int,int,int);

private:
   std::thread threadTrem;
   int id;
   int x;
   int y;
   int velocidade;
   bool enable;
   int iniX, iniY, fimX, fimY; // determina o x e y iniciais e finais do retangulo
   const static int ladoQuadrado = 20;
   const static int alturaTrilho = 100;
   const static int baseTrilho = 140;
};

#endif // TREM_H



