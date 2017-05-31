#include <QApplication>
#include <iostream>
#include "setupdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SetupDialog setupDialog;
    setupDialog.show();

    return a.exec();
}
