#-------------------------------------------------
#
# Project created by QtCreator 2014-08-12T22:04:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 2048
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    playground.cpp \
    node.cpp

HEADERS  += mainwindow.h \
    playground.h \
    node.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -std=c++0x
