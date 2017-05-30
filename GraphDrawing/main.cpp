#include "RailroadsViewer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RailroadsViewer w(10, 10);
    w.show();

    return a.exec();
}
