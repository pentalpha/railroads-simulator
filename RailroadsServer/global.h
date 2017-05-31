#ifndef GLOBAL_H
#define GLOBAL_H
#include <iostream>
#include <string>
#include <qwidget.h>

using namespace std;

class Global
{
public:
    Global();
    static void startServer(QWidget* parent, int port, string ip, string graphPath);
};

#endif // GLOBAL_H
