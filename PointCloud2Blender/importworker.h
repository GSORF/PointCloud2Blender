/*
    PointCloud2Blender - convert point cloud files to a textured 3D mesh
    Copyright (C) 2015 Adam Kalisz (Bachelor@Kalisz.co)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef IMPORTWORKER_H
#define IMPORTWORKER_H

#include <QObject>
#include <QRunnable>
#include <QThread>
#include <QDebug>

#include <QFile>

#include "panorama3d.h"
#include "glmesh.h"
#include "glwidget.h"

class Point3D;
class Panorama3D;
class GLWidget;

class ImportWorker : public QObject, public QRunnable
{
    Q_OBJECT
public:
    enum FileType
    {
        XYZ_ASCII,
        XYZ_BINARY,
        PLY
    };


    explicit ImportWorker(Panorama3D *panorama, GLWidget *glWidget, QString fileName, bool analyze, QObject *parent = 0);
    ~ImportWorker();

    void run();
    void import_XYZ_Ascii_File();
    void import_XYZ_Binary_File();
    void import_PLY_File();
    bool determineOriginalResolution(Point3D newPoint);

    Panorama3D *panorama;
    GLWidget *glWidget;
    QTimer updateTimer;
    FileType fileType;
    QString fileName;
    bool analyze;
    QVector<int> histogram_horizontal_angles;
    float angle_accuracy;

    bool cancelThread;

    void stopThread();

signals:
    void newPoint(Point3D point);
    void importStatus(float percent);
    void originalResolution(int horizontal);

    void showInfoMessage(QString message);
    void showErrorMessage(QString message);

public slots:

};

#endif // IMPORTWORKER_H
