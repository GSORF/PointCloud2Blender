#ifndef PANORAMA3D_H
#define PANORAMA3D_H

#include <QObject>

#include <QtMath>
#include <QImage>

#include "importworker.h"

class Panorama3D : public QObject
{
    Q_OBJECT

public:
    enum Orientation
    {
        LEFT_UP_X,
        LEFT_UP_Y,
        LEFT_UP_Z,
        RIGHT_UP_X,
        RIGHT_UP_Y,
        RIGHT_UP_Z
    };

    explicit Panorama3D(Orientation upVector, quint8 resolution, QObject *parent = 0);
    void finished();

private:
    Orientation upVector;
    quint8 resolution;

    QImage panoramaDepth;
    QImage panoramaColor;

signals:
    void updateDepthMap(QImage *depthMap);
    void updateColorMap(QImage *colorMap);

public slots:
    void newPoint(Point3D point);

};

#endif // PANORAMA3D_H
