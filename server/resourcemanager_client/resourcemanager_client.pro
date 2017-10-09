#-------------------------------------------------
#
# Project created by QtCreator 2017-09-16T22:31:30
#
#-------------------------------------------------
QT       += widgets
QT       += core gui
QT       += network

TARGET = resourcemanager_client
TEMPLATE = app
RC_FILE = app.rc

SOURCES += main.cpp\
        mainwindow.cpp \
    msgtipsautoshut.cpp \
    msgtips.cpp \
    sockthread.cpp \
    keybutton.cpp

HEADERS  += mainwindow.h \
    msgtipsautoshut.h \
    msgtips.h \
    sockthread.h \
    kouling.h \
    keybutton.h

FORMS    += mainwindow.ui

OTHER_FILES +=

RESOURCES += \
    qss.qrc
