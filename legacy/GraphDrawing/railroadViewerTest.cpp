#include "RailroadsViewer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::string graphFilePath = "/home/pentalpha/main/UFRN/2017.1/Lab. Projeto de S.O./railroads-simulator/min.graph";
    int zoom = 10;
    int padding = 40;
    RailroadsViewer railroads(zoom, padding, graphFilePath, true);
    railroads.show();

    return a.exec();
}
