#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "trem.h"
#include <QMainWindow>
#include <QList>

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void updateInterface(int,int,int);

private:
    Ui::MainWindow *ui;
    QList<Trem*> trens;
};

#endif // MAINWINDOW_H
