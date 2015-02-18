#include "panorama3d.h"

Panorama3D::Panorama3D(Orientation upVector, quint8 resolution, QObject *parent) :
    QObject(parent)
{
    this->upVector = upVector;
    this->resolution = resolution;

    panoramaDepth = QImage(360 * resolution, 180 * resolution, QImage::Format_ARGB32_Premultiplied);
    panoramaColor = QImage(360 * resolution, 180 * resolution, QImage::Format_ARGB32_Premultiplied);


}

void Panorama3D::finished()
{
    panoramaDepth.save("/home/akp/Desktop/depthmap.jpg");
    panoramaColor.save("/home/akp/Desktop/colormap.jpg");
}

void Panorama3D::newPoint(Point3D point)
{
    //Calculate spherical coordinates and change the pixels of the panoramas respectively

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

    float maxDistance = 300.0f;

    //qDebug() << "newPoint: (" << point.x << "," << point.y << "," << point.z << ") => (" << radius << "," << theta_degrees << "," << phi_degrees << ")";

    //Depth image:
    QRgb depthValue = qRgb( (radius/maxDistance)*255, (radius/maxDistance)*255, (radius/maxDistance)*255 );
    panoramaDepth.setPixel(phi_degrees*resolution, theta_degrees*resolution, depthValue);

    //qDebug() << "depthValue: " << depthValue;

    //Color image:
    QRgb colorValue = qRgb( point.r, point.g, point.b );
    panoramaColor.setPixel(phi_degrees*resolution, theta_degrees*resolution, colorValue);

    //qDebug() << "colorValue: " << colorValue;

    //Update the images in the user interface
    emit updateDepthMap(&panoramaDepth);
    emit updateColorMap(&panoramaColor);

}
