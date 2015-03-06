#include "meshworker.h"

MeshWorker::MeshWorker(Panorama3D *panorama, QObject *parent) : QObject(parent)
{
    this->panorama = panorama;

    this->meshing = false;
    this->maxTiles = 1;
    this->currentTile = 0;

    qDebug() << "MeshWorker::MeshWorker(Panorama3D *panorama, QObject *parent) : QObject(parent)";
}

MeshWorker::~MeshWorker()
{
    qDebug() << "MeshWorker::~MeshWorker()";
}



void MeshWorker::run()
{
    this->meshing = true;
    qDebug() << "MeshWorker::run()";

    int width = this->panorama->panoramaDepth.width();
    int height = this->panorama->panoramaDepth.height();

    Point3D newPoint;

    while(this->meshing)
    {
        for(; this->currentTile < this->maxTiles; this->currentTile++)
        {
            qDebug() << "this->currentTile" << this->currentTile << "this->maxTiles" << this->maxTiles;

            QString filename_mtl, filename_obj;
            filename_mtl = filename_obj = QDateTime::currentDateTime().toString("hh_mm_ss") + "_tile_" + QString::number(this->currentTile) + ".obj";
            filename_mtl.replace("obj", "mtl");

            QFile file( QDir::currentPath() + "/" + filename_obj);

            if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                qDebug() << "Cannot open file for writing: " << filename_obj;
                return;
            }

            /*
             OBJ-FIle Format:

            o panorama_mesh
            mtllib filename_mtl.mtl
            usemtl panorama

            v 0.000000 0.000000 2.000000
            v 0.000000 0.000000 0.000000
            v 2.000000 0.000000 0.000000
            v 2.000000 0.000000 2.000000
            vt 1.0  0.5
            vt 0.0  1.0
            vt 0.5  1.0
            vt 1.0  1.0
            f -4 -3 -2 -1

              */


            QTextStream outputStream(&file);
            outputStream << "mtllib " << filename_mtl << "\n";
            outputStream << "o " << filename_obj << "\n";
            outputStream << "usemtl panorama\n\n";

            for(int x = 0; x < width-1; x++)
            {
                for(int y = 0; y < height-1; y++)
                {
                    if(! QColor(this->panorama->panoramaDepth.pixel(x,y)).value() == 0)
                    {
                        //create quad clockwise:
                        projectPanorama3D(x,y,newPoint);
                        outputStream << "v " << newPoint.x << " " << newPoint.y << " " << newPoint.z << "\n";
                        outputStream << "vt " << x / (width * 1.0f) << " " << y / (height * 1.0f) << "\n";
                        emit addPoint(newPoint);

                        projectPanorama3D(x+1,y,newPoint);
                        outputStream << "v " << newPoint.x << " " << newPoint.y << " " << newPoint.z << "\n";
                        outputStream << "vt " << (x+1) / (width * 1.0f) << " " << y / (height * 1.0f) << "\n";

                        projectPanorama3D(x+1,y+1,newPoint);
                        outputStream << "v " << newPoint.x << " " << newPoint.y << " " << newPoint.z << "\n";
                        outputStream << "vt " << (x+1) / (width * 1.0f) << " " << (y+1) / (height * 1.0f) << "\n";

                        projectPanorama3D(x,y+1,newPoint);
                        outputStream << "v " << newPoint.x << " " << newPoint.y << " " << newPoint.z << "\n";
                        outputStream << "vt " << x / (width * 1.0f) << " " << (y+1) / (height * 1.0f) << "\n";

                        outputStream << "f -8 -6 -4 -2\n";

                    }
                }

            }


            file.close();

            /*
             MTL-Example

            newmtl panorama
            Ns 10.0000
            Ni 1.5000
            d 1.0000
            Tr 0.0000
            Tf 1.0000 1.0000 1.0000
            illum 2
            Ka 0.0000 0.0000 0.0000
            Kd 0.5880 0.5880 0.5880
            Ks 0.0000 0.0000 0.0000
            Ke 0.0000 0.0000 0.0000
            map_Ka colormap.jpg
            map_Kd colormap.jpg


              */

            QFile file2( QDir::currentPath() + "/" + filename_mtl);

            if(!file2.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                qDebug() << "Cannot open file for writing: " << filename_mtl;
                return;
            }

            QTextStream outputStream2(&file2);
            outputStream2 << "newmtl " << filename_mtl << "\n"
                          << "Ns 10.0000\n"
                          << "Ni 1.5000\n"
                          << "d 1.0000\n"
                          << "Tr 0.0000\n"
                          << "Tf 1.0000 1.0000 1.0000\n"
                          << "illum 2\n"
                          << "Ka 0.0000 0.0000 0.0000\n"
                          << "Kd 0.5880 0.5880 0.5880\n"
                          << "Ks 0.0000 0.0000 0.0000\n"
                          << "Ke 0.0000 0.0000 0.0000\n"
                          << "map_Ka colormap.jpg\n"
                          << "map_Ka colormap.jpg\n";
            file2.close();
        }
        this->meshing = false;
    }

}

void MeshWorker::projectPanorama3D(int x, int y, Point3D &projectedPoint)
{
    QColor depthValue = QColor( this->panorama->panoramaDepth.pixel(x,y) );
    QColor colorValue = QColor( this->panorama->panoramaColor.pixel(x,y) );
    int z_depth = depthValue.value();

    float degree_horizontal = x / this->panorama->resolution;
    float degree_vertical = y / this->panorama->resolution;
    float radian_horizontal = qDegreesToRadians(degree_horizontal);
    float radian_vertical = qDegreesToRadians(degree_vertical);

    projectedPoint.x = qSin(radian_horizontal) * z_depth;
    projectedPoint.y = qCos(radian_horizontal) * z_depth;
    projectedPoint.z = qCos(radian_vertical) * z_depth;
    projectedPoint.r = colorValue.red();
    projectedPoint.g = colorValue.green();
    projectedPoint.b = colorValue.blue();



}
