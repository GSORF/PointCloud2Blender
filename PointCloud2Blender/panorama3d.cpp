#include "panorama3d.h"

Panorama3D::Panorama3D(QVector3D translationVector, Orientation upVector, quint8 resolution, QObject *parent) :
    QObject(parent)
{
    this->translationVector = translationVector;
    this->upVector = upVector;
    this->resolution = resolution;

    panoramaDepth = QImage(360 * resolution, 180 * resolution, QImage::Format_ARGB32_Premultiplied);
    panoramaColor = QImage(360 * resolution, 180 * resolution, QImage::Format_ARGB32_Premultiplied);
}

void Panorama3D::finished()
{
    qDebug() << "Saving panoramas into " << QDir::currentPath();

    panoramaDepth.save(QDir::currentPath() + "/depthmap.jpg");
    panoramaColor.save(QDir::currentPath() + "/colormap.jpg");

    //Update the images in the user interface
    emit updateDepthMap(&panoramaDepth);
    emit updateColorMap(&panoramaColor);
}

void Panorama3D::addPoint(Point3D point)
{
    //Calculate spherical coordinates and change the pixels of the panoramas respectively
    point.x += translationVector.x();
    point.y += translationVector.y();
    point.z += translationVector.z();

    //Distance
    float radius = qSqrt( point.x*point.x + point.y*point.y + point.z*point.z );

    if(!radius > 0 || point.x == 0)
        return;

    //Normalize Coordinates
    float x = point.x / radius;
    float y = point.y / radius;
    float z = point.z / radius;

    //Inclination (theta)
    float theta = qAcos( z / radius ); //Range: 0*PI <= theta <= 1*PI
    //Azimuth (phi)
    float phi = qAtan2(y, x); //Atan2 Range is -1*PI <= phi <= 1*PI (Note: Atan Range is (-PI/2, PI/2) !!!)

    if(phi < 0) //Map phi to (0*PI, 2*PI)
    {
        //phi = (phi + 2*M_PI) % (2*M_PI);
        phi = phi + M_PI;
    }

    //Convert to degrees
    float theta_degrees = qRadiansToDegrees(theta);
    float phi_degrees = qRadiansToDegrees(phi);

    //The maximum scanned distance:
    float maxDistance = 60.0f;

    //qDebug() << "newPoint: (" << point.x << "," << point.y << "," << point.z << ") => (" << radius << "," << theta_degrees << "," << phi_degrees << ")";

    //Depth image:
    QRgb depthValue = qRgb( (radius/maxDistance)*255, (radius/maxDistance)*255, (radius/maxDistance)*255 );
    panoramaDepth.setPixel(phi_degrees*resolution, theta_degrees*resolution, depthValue);

    //qDebug() << "depthValue: " << depthValue;

    //Color image:
    QRgb colorValue = qRgb( point.r, point.g, point.b );
    panoramaColor.setPixel(phi_degrees*resolution, theta_degrees*resolution, colorValue);

    //qDebug() << "colorValue: " << colorValue;

}
