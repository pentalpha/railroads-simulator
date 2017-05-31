#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Trem *trem = new Trem(1,180,120, 160, 130, 300, 230);
    trens.push_back(new Trem(1,120,130, 110, 140, 310, 220));
    trens.push_back(new Trem(2,330,130, 320, 140, 520, 220));
    trens.push_back(new Trem(3,550,160, 530, 170, 610, 370));
    trens.push_back(new Trem(4,420,390, 320, 320, 520, 400));
    trens.push_back(new Trem(5,220,270, 210, 230, 410, 310));
    trens.push_back(new Trem(6,220,390, 110, 320, 310, 400));
    trens.push_back(new Trem(7,10,270, 20, 170, 100, 370));
    connect(trens[0],SIGNAL(updateGUI(int,int,int)),SLOT(updateInterface(int,int,int)));
    connect(trens[1],SIGNAL(updateGUI(int,int,int)),SLOT(updateInterface(int,int,int)));
    connect(trens[2],SIGNAL(updateGUI(int,int,int)),SLOT(updateInterface(int,int,int)));
    connect(trens[3],SIGNAL(updateGUI(int,int,int)),SLOT(updateInterface(int,int,int)));
    connect(trens[4],SIGNAL(updateGUI(int,int,int)),SLOT(updateInterface(int,int,int)));
    connect(trens[5],SIGNAL(updateGUI(int,int,int)),SLOT(updateInterface(int,int,int)));
    connect(trens[6],SIGNAL(updateGUI(int,int,int)),SLOT(updateInterface(int,int,int)));
    trens[0]->start();
    trens[1]->start();
    trens[2]->start();
    trens[3]->start();
    trens[4]->start();
    trens[5]->start();
    trens[6]->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateInterface(int id, int x, int y)
{
    switch(id){
        case 1:
            ui->labelTrem01->setGeometry(x,y,20,20);
            break;
        case 2:
            ui->labelTrem02->setGeometry(x,y,20,20);
            break;
        case 3:
            ui->labelTrem03->setGeometry(x,y,20,20);
            break;
        case 4:
            ui->labelTrem04->setGeometry(x,y,20,20);
            break;
        case 5:
            ui->labelTrem05->setGeometry(x,y,20,20);
            break;
        case 6:
            ui->labelTrem06->setGeometry(x,y,20,20);
            break;
        case 7:
            ui->labelTrem07->setGeometry(x,y,20,20);
            break;
        default:
            break;
    }
}

