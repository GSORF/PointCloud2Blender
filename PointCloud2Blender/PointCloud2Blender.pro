#-------------------------------------------------
#
# Project created by QtCreator 2015-02-15T20:41:11
#
#-------------------------------------------------

QT       += core gui opengl
CONFIG   += console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PointCloud2Blender
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    importworker.cpp \
    panorama3d.cpp \
    glwidget.cpp \
    glmesh.cpp \
    meshworker.cpp

HEADERS  += mainwindow.h \
    importworker.h \
    panorama3d.h \
    glwidget.h \
    glmesh.h \
    meshworker.h

FORMS    += mainwindow.ui

RESOURCES += \
    ressource.qrc

DISTFILES +=
