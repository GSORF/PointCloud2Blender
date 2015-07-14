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

#ifndef MESHWORKER_H
#define MESHWORKER_H

#include <QObject>
#include <QRunnable>
#include <QThread>
#include <QDebug>
#include <QImage>
#include <QColor>
#include <QDateTime>

#include "panorama3d.h"


class MeshWorker : public QObject, public QRunnable
{
    Q_OBJECT
public:
    MeshWorker(Panorama3D *panorama, GLWidget *glWidget, float normalAngleThreshold, QObject *parent = 0);
    ~MeshWorker();

    void run();

    Panorama3D *panorama;
    GLWidget *glWidget;

    bool meshing;

    int maxTiles;
    int currentTile;

    float normalAngleThreshold;

    bool cancelThread;

    void stopThread();

signals:
    void meshingStatus(float percent);
};

#endif // MESHWORKER_H
