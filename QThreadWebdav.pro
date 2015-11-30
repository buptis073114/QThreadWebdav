#-------------------------------------------------
#
# Project created by QtCreator 2015-11-27T21:39:49
#
#-------------------------------------------------

QT       += core gui

QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QThreadWebdav
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    webdavthread.cpp

HEADERS  += mainwindow.h \
    webdavthread.h

FORMS    += mainwindow.ui
