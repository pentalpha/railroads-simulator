#include "RailroadsViewer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RailroadsViewer w(10, 40);
    w.show();

    return a.exec();
}
