#-------------------------------------------------
#
# Project created by QtCreator 2017-09-13T17:09:18
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MegaStat
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    xmllibrary.cpp \
    cidentry.cpp

HEADERS  += mainwindow.h \
    xmllibrary.h \
    cidentry.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc
