#ifndef PANORAMA3D_H
#define PANORAMA3D_H

#include <QObject>

#include <QtMath>
#include <QImage>
#include <QDir>
#include <QVector3D>

#include "importworker.h"

class Point3D
{
    //Example xyz file (original Faro Scene):
    //-59.43620000 -31.36650000 302.80950000 59 46 55

public:
    //Position Data
    float x,y,z;
    //Color Data
    quint16 r,g,b;

    QString toString()
    {
        return "(" + QString::number(x) + ", "  + QString::number(y) + ", " + QString::number(z) + ")";
    }
};


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

    enum ProjectionType
    {
        EQUIRECTANGULAR,
        CYLINDRICAL,
        MERCATOR
    };

    explicit Panorama3D(QVector3D translationVector, Orientation upVector, quint8 resolution, float maxDistance, ProjectionType projectionType, QObject *parent = 0);
    void finished();

private:
    QVector3D translationVector;
    Orientation upVector;
    quint8 resolution;
    float maxDistance;
    ProjectionType projectionType;

    float minRadius;
    float maxRadius;
    float minX;
    float maxX;
    float minY;
    float maxY;

    void project(float theta, float phi, float &x, float &y);

    QImage panoramaDepth;
    QImage panoramaColor;

signals:
    void updateDepthMap(QImage *depthMap);
    void updateColorMap(QImage *colorMap);

public slots:
    void addPoint(Point3D point);


};

#endif // PANORAMA3D_H
