#-------------------------------------------------
#
# Project created by QtCreator 2017-09-16T21:27:52
#
#-------------------------------------------------

QT       += widgets
QT       += core gui
QT       += network

TARGET = resourcemanager_server
TEMPLATE = app
RC_FILE = app.rc


SOURCES += main.cpp\
        mainwindow.cpp \
    msgtips.cpp \
    msgtipsautoshut.cpp \
    sockthread.cpp

HEADERS  += mainwindow.h \
    msgtips.h \
    msgtipsautoshut.h \
    kouling.h \
    sockthread.h

FORMS    += mainwindow.ui
