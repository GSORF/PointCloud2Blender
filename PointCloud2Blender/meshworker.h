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
    MeshWorker(Panorama3D *panorama, QObject *parent = 0);
    ~MeshWorker();

    void run();

    Panorama3D *panorama;

    bool meshing;

    int maxTiles;
    int currentTile;

    void projectPanorama3D(int x, int y, Point3D &projectedPoint);

signals:
    void addPoint(Point3D newPoint);
};

#endif // MESHWORKER_H
