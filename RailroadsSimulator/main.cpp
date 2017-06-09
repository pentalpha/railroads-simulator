#include <QApplication>
#include <iostream>
#include "setupdialog.h"
#include "global.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SetupDialog setupDialog;
    setupDialog.show();

    auto execReturn = a.exec();

    Global::destroyAll();

    return execReturn;
}
