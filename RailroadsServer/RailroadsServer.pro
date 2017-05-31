QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = RailroadsServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    QSFMLCanvas.cpp \
    rail.cpp \
    RailroadsCanvas.cpp \
    RailroadsViewer.cpp \
    RailsGraph.cpp \
    trainposindicator.cpp \
    setupdialog.cpp \
    global.cpp \
    Queue.cpp \
    StringQueue.cpp \
    Server.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DISTFILES +=

HEADERS += \
    QSFMLCanvas.h \
    rail.h \
    RailroadsCanvas.h \
    RailroadsViewer.h \
    RailsGraph.h \
    trainposindicator.h \
    setupdialog.h \
    global.h \
    Queue.h \
    StringQueue.h \
    Server.h

### TO ALLOW QSFMLCanvas ###
INCLUDEPATH += /usr/include/SFML
DEPENDPATH += /usr/include/SFML
CONFIG(release, debug|release):
LIBS += -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-window -lsfml-system
CONFIG(debug, debug|release):
LIBS += -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-window -lsfml-system

FORMS += \
    setupdialog.ui
