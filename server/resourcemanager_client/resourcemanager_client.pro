#-------------------------------------------------
#
# Project created by QtCreator 2017-09-16T22:31:30
#
#-------------------------------------------------

QT       += core gui
QT       += network

TARGET = resourcemanager_client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    msgtipsautoshut.cpp \
    msgtips.cpp \
    sockthread.cpp

HEADERS  += mainwindow.h \
    msgtipsautoshut.h \
    msgtips.h \
    sockthread.h

FORMS    += mainwindow.ui
