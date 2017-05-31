#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Trem *trem = new Trem(1,180,120, 160, 130, 300, 230);
    trens.push_back(new Trem(1,130,140, 160, 130, 300, 230));
    connect(trens[0],SIGNAL(updateGUI(int,int,int)),SLOT(updateInterface(int,int,int)));
    trens[0]->start();
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
            break;
        default:
            break;
    }
}

