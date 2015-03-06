#include "panorama3d.h"

Panorama3D::Panorama3D(QVector3D translationVector, Orientation upVector, quint8 resolution, float maxDistance, ProjectionType projectionType, QObject *parent) :
    QObject(parent)
{
    this->translationVector = translationVector;
    this->upVector = upVector;
    this->resolution = resolution;
    this->maxDistance = maxDistance;
    this->projectionType = projectionType;

    panoramaDepth = QImage(360 * resolution, 180 * resolution, QImage::Format_ARGB32_Premultiplied);
    panoramaColor = QImage(360 * resolution, 180 * resolution, QImage::Format_ARGB32_Premultiplied);

    minRadius = 500;
    maxRadius = 0;
    minY = 180;
    maxY = 0;
    minX = 360;
    maxX = 0;
}

void Panorama3D::finished()
{
    qDebug() << "Saving panoramas into " << QDir::currentPath();

    panoramaDepth.save(QDir::currentPath() + "/depthmap.jpg");
    panoramaColor.save(QDir::currentPath() + "/colormap.jpg");

    //Update the images in the user interface
    emit updateDepthMap(&panoramaDepth);
    emit updateColorMap(&panoramaColor);

    qDebug() << "minRadius="<<minRadius<<"maxRadius="<<maxRadius<<"minX="<<minX<<"maxX="<<maxX<<"minY="<<minY<<"maxY="<<maxY;
}

void Panorama3D::project(float theta, float phi, float &x, float &y)
{
    switch (this->projectionType) {
    default:
    case EQUIRECTANGULAR:
        x = theta;
        y = phi;
        break;
    case CYLINDRICAL:
        x = theta;
        y = qTan(phi) + M_PI;
        break;
    case MERCATOR:
        x = theta;
        y = qLn( qTan(phi) + (1/qCos(phi)) ); //this will generate huge negative values for an argument < 1
        break;
    }
}

void Panorama3D::addPoint(Point3D point)
{
    //Calculate spherical coordinates and change the pixels of the panoramas respectively
    point.x += translationVector.x();
    point.y += translationVector.y();
    point.z += translationVector.z();

    //Distance
    float radius = qSqrt( point.x*point.x + point.y*point.y + point.z*point.z );
    float phi, theta;

    if(!radius > 0 || point.x == 0)
        return;

    //Note: There are different definitions about phi and theta (mathematical/physical). To match it with the projections, we will use the following:

    switch (upVector) {
    default:
    case RIGHT_UP_Z:
    case LEFT_UP_Z:
        //Inclination (phi)
        phi = qAcos( point.z / radius ); //Range: 0*PI <= phi <= 1*PI
        //Azimuth (theta)
        theta = qAtan2(point.y, point.x); //Atan2 Range is -1*PI <= theta <= 1*PI (Note: Atan Range is (-PI/2, PI/2) !!!)
        break;
    case RIGHT_UP_Y:
    case LEFT_UP_Y:
        //Inclination (phi)
        phi = qAcos( point.y / radius ); //Range: 0*PI <= phi <= 1*PI
        //Azimuth (theta)
        theta = qAtan2(-point.z, point.x); //Atan2 Range is -1*PI <= theta <= 1*PI (Note: Atan Range is (-PI/2, PI/2) !!!)
        break;
    case RIGHT_UP_X:
    case LEFT_UP_X:
        //Inclination (phi)
        phi = qAcos( point.x / radius ); //Range: 0*PI <= phi <= 1*PI
        //Azimuth (theta)
        theta = qAtan2(point.y, -point.z); //Atan2 Range is -1*PI <= theta <= 1*PI (Note: Atan Range is (-PI/2, PI/2) !!!)
        break;
    }


    //Map theta to (0*PI, 2*PI)
    theta = theta + M_PI;

    //Project spherical coordinates on 2D plane and return coordinates:
    float x, y;
    project(theta, phi, x, y);

    //Convert to degrees
    x = qRadiansToDegrees(x);
    y = qRadiansToDegrees(y);

    if(radius < minRadius)
    {
        minRadius = radius;
    }
    else if(radius > maxRadius)
    {
        maxRadius = radius;
    }

    if(x < minX)
    {
        minX = x;
    }
    else if(x > maxX)
    {
        maxX = x;
    }

    if(y < minY)
    {
        minY = y;
    }
    else if(y > maxY)
    {
        maxY = y;
    }

    //Depth image:
    quint8 depth = (radius/maxDistance)*255;
    QRgb depthValue = qRgba( depth, depth, depth, 255 );
    panoramaDepth.setPixel(x*resolution, y*resolution, depthValue);

    //Color image:
    QRgb colorValue = qRgba( point.r, point.g, point.b, 255 );
    panoramaColor.setPixel(x*resolution, y*resolution, colorValue);
}
