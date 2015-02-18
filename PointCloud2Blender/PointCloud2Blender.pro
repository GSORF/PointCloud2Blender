#-------------------------------------------------
#
# Project created by QtCreator 2015-02-15T20:41:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PointCloud2Blender
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    importworker.cpp \
    panorama3d.cpp

HEADERS  += mainwindow.h \
    importworker.h \
    panorama3d.h

FORMS    += mainwindow.ui

RESOURCES += \
    ressource.qrc
