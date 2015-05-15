#include "panorama3d.h"

Panorama3D::Panorama3D(QVector3D translationVector, Orientation upVector, const int mapWidth, const int mapHeight, float maxDistance, ProjectionType projectionType, QObject *parent) :
    QObject(parent)
{
    this->translationVector = translationVector;
    this->upVector = upVector;
    this->mapWidth = mapWidth;
    this->mapHeight = mapHeight;
    this->maxDistance = maxDistance;
    this->projectionType = projectionType;

    panoramaDepth = QImage(mapWidth, mapHeight, QImage::Format_ARGB32);
    panoramaColor = QImage(mapWidth, mapHeight, QImage::Format_ARGB32);
    floatDepthMap.resize(mapWidth * mapHeight);

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
    refreshTextureMapsGUI();

    qDebug() << "minRadius="<<minRadius<<"maxRadius="<<maxRadius<<"minX="<<minX<<"maxX="<<maxX<<"minY="<<minY<<"maxY="<<maxY;
}

bool Panorama3D::convertToSpherical(Point3D &point, float &theta, float &phi, float &radius)
{
    //Calculate spherical coordinates
    point.x += translationVector.x();
    point.y += translationVector.y();
    point.z += translationVector.z();

    //Distance
    radius = qSqrt( point.x*point.x + point.y*point.y + point.z*point.z );

    if(!radius > 0.0f || point.x == 0.0f)
        return false;

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

    return true;
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

void Panorama3D::unprojectPanorama3D(int x, int y, Point3D &projectedPoint)
{
    QColor depthValue = QColor( this->panoramaDepth.pixel(x,y) );
    QColor colorValue = QColor( this->panoramaColor.pixel(x,y) );
    int z_depth = depthValue.value();

    float degree_horizontal = x / (mapWidth / 360.0f);
    float degree_vertical = y / (mapHeight / 180.0f);
    float radian_horizontal = qDegreesToRadians(degree_horizontal);
    float radian_vertical = qDegreesToRadians(degree_vertical);

    projectedPoint.x = z_depth * qSin(radian_vertical) * qCos(radian_horizontal);
    projectedPoint.y = z_depth * qSin(radian_vertical) * qSin(radian_horizontal);
    projectedPoint.z = z_depth * qCos(radian_vertical);

    projectedPoint.r = colorValue.red();
    projectedPoint.g = colorValue.green();
    projectedPoint.b = colorValue.blue();
}

QVector3D Panorama3D::getTranslationVector()
{
    return this->translationVector;
}

void Panorama3D::addPoint(Point3D point)
{
    float phi, theta, radius;

    //Calculate spherical coordinates
    if(!convertToSpherical(point, theta, phi, radius))
        return;


    //Project spherical coordinates on 2D plane and return coordinates:
    float x, y;
    project(theta, phi, x, y);

    //Convert to degrees
    x = qRadiansToDegrees(x);
    y = qRadiansToDegrees(y);

    //Bugfix: Out of Range:
    if(x < 0.0f || x >= 360.0f) return;
    if(y < 0.0f || y >= 180.0f) return;

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

    //Change the pixels of the panoramas respectively

    //Depth image:
    int depth = (radius/maxDistance)*255;

    //Bugfix: "Depth-Modulus" -> limit depth
    if(depth > 255) depth = 255;

    QColor depthValue = QColor( qRgba( depth, depth, depth, 255 ) );

    //Bugfix: Repeated gradients in DepthImage result in non-contigous depth:
    //TODO: BETTER in future, save as float instead of int!
    QColor depthMapValue = QColor( panoramaDepth.pixel(x*(mapWidth / 360.0f), y*(mapHeight / 180.0f)));
    if( depthMapValue.value() != 0 && depthMapValue.value() < depthValue.value())
    {
        return;
    }

    panoramaDepth.setPixel(x*(mapWidth / 360.0f), y*(mapHeight / 180.0f), depthValue.rgba());

    //Color image:
    QColor colorValue = QColor( qRgba( point.r, point.g, point.b, 255 ));
    panoramaColor.setPixel(x*(mapWidth / 360.0f), y*(mapHeight / 180.0f), colorValue.rgba());
}

void Panorama3D::refreshTextureMapsGUI()
{
    qDebug() << "Refreshing Texture Maps";
    emit updateDepthMap(&panoramaDepth);
    emit updateColorMap(&panoramaColor);
}
