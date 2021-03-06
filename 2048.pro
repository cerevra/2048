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
    node.cpp \
    about.cpp \
    settings.cpp \
    styles.cpp \
    move_p.cpp \
    move.cpp

HEADERS  += mainwindow.h \
    playground.h \
    node.h \
    about.h \
    settings.h \
    styles.h \
    move.h \
    move_p.h

FORMS    += mainwindow.ui \
    about.ui \
    settings.ui

QMAKE_CXXFLAGS += -std=c++0x

TRANSLATIONS   += tr_ru.ts
