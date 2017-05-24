#-------------------------------------------------
#
# Project created by QtCreator 2016-04-15T06:45:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TrabalhoTrem
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    trem.cpp

HEADERS  += mainwindow.h \
    trem.h

FORMS    += mainwindow.ui

CONFIG  += c++11
LIBS += -pthread
