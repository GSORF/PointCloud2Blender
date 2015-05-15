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

signals:
    void meshingStatus(int percent);
};

#endif // MESHWORKER_H
