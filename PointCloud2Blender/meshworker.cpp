#include "meshworker.h"

MeshWorker::MeshWorker(Panorama3D *panorama, GLWidget *glWidget, float normalAngleThreshold, QObject *parent) : QObject(parent)
{
    this->panorama = panorama;
    this->glWidget = glWidget;

    this->meshing = false;
    this->maxTiles = 1;
    this->currentTile = 0;

    this->normalAngleThreshold = normalAngleThreshold;

    this->cancelThread = false;

    //Preventing crash due to unhandled signals from mesher
    this->setAutoDelete(false);

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

    while(this->meshing && !this->cancelThread)
    {
        for(; this->currentTile < this->maxTiles; this->currentTile++)
        {
            //qDebug() << "this->currentTile" << this->currentTile << "this->maxTiles" << this->maxTiles;

            QString filename_mtl, filename_obj;
            filename_mtl = filename_obj = this->panorama->mapFilename + "_tile_" + QString::number(this->currentTile) + ".obj";
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
            outputStream << "# " << QApplication::applicationName() << " v" << QApplication::applicationVersion() << " OBJ File\n";
            outputStream << "# http://bachelor.kalisz.co\n";
            outputStream << "mtllib " << filename_mtl << "\n";
            outputStream << "o " << filename_obj << "\n";
            outputStream << "usemtl panorama\n\n";

            for(int x = 0; x < width; x++)
            {
                if(this->cancelThread) break;

                for(int y = 0; y < height; y++)
                {
                    if(this->cancelThread) break;

                    if(! QColor(this->panorama->panoramaDepth.pixel(x,y)).value() == 0)
                    {
                        //create quad clockwise:
                        int x1, x2, x3, x4;
                        int y1, y2, y3, y4;

                        //Top left:
                        x1 = x;
                        y1 = y;
                        //Top right:
                        x2 = x+1;
                        y2 = y;
                        //Bottom right:
                        x3 = x+1;
                        y3 = y+1;
                        //Bottom left:
                        x4 = x;
                        y4 = y+1;

                        //Check for bounds, if reached border, wrap around back (prevents polystrip gap):
                        if(x == width - 1)
                        {
                            x2 = 0;
                            x3 = 0;
                        }
                        if(y == height - 1)
                        {
                            x2 = 0;
                            x3 = 0;
                            y3 = 0;
                            y4 = 0;
                        }

                        Point3D v1, v2, v3, v4;
                        panorama->unprojectPanorama3D(x1, y1, v1);
                        panorama->unprojectPanorama3D(x2, y2, v2);
                        panorama->unprojectPanorama3D(x3, y3, v3);
                        panorama->unprojectPanorama3D(x4, y4, v4);

                        //Avoid deformed faces due to one black pixel (v1 cannot be null):
                        if( v2.isNull() )
                        {
                            v2.x = (v1.x + v3.x + v4.x) / 3.0f;
                            v2.y = (v1.y + v3.y + v4.y) / 3.0f;
                            v2.z = (v1.z + v3.z + v4.z) / 3.0f;
                        }
                        if( v3.isNull() )
                        {
                            v3.x = (v1.x + v2.x + v4.x) / 3.0f;
                            v3.y = (v1.y + v2.y + v4.y) / 3.0f;
                            v3.z = (v1.z + v2.z + v4.z) / 3.0f;
                        }
                        if( v4.isNull() )
                        {
                            v4.x = (v1.x + v2.x + v3.x) / 3.0f;
                            v4.y = (v1.y + v2.y + v3.y) / 3.0f;
                            v4.z = (v1.z + v2.z + v3.z) / 3.0f;
                        }

                        //Generate normal vectors (one for each triangle):
                        QVector3D v1v2(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
                        QVector3D v1v4(v4.x - v1.x, v4.y - v1.y, v4.z - v1.z);
                        QVector3D v3v2(v2.x - v3.x, v2.y - v3.y, v2.z - v3.z);
                        QVector3D v3v4(v4.x - v3.x, v4.y - v3.y, v4.z - v3.z);
                        QVector3D normal1 = QVector3D::crossProduct(v1v2, v1v4);
                        QVector3D normal2 = QVector3D::crossProduct(v3v2, v3v4);
                        float area1 = normal1.length() / 2.0f;
                        float area2 = normal2.length() / 2.0f;

                        //Measure the angle between normal and v1:
                        QVector3D myV1(-v1.x, -v1.y, -v1.z);
                        normal1.normalize();
                        normal2.normalize();
                        myV1.normalize();

                        float cosine_of_angle1 = QVector3D::dotProduct(myV1, normal1);
                        float cosine_of_angle2 = QVector3D::dotProduct(myV1, normal2);


                        if( qAbs( cosine_of_angle1 ) < qAbs( qCos(normalAngleThreshold) ) || qAbs( cosine_of_angle2 ) < qAbs( qCos(normalAngleThreshold) ) )
                        {
                            //qDebug() << "face discarded due to bad angle";
                        }
                        else if( area1 < 0.005f || area2 < 0.005f)
                        {
                            //qDebug() << "face discarded due to almost degenerate face";
                        }
                        else
                        {
                            outputStream << "v " << v1.x << " " << v1.y << " " << v1.z << "\n";
                            glWidget->addPoint(v1, panorama->getTranslationVector());

                            outputStream << "v " << v2.x << " " << v2.y << " " << v2.z << "\n";
                            glWidget->addPoint(v2, panorama->getTranslationVector());

                            outputStream << "v " << v3.x << " " << v3.y << " " << v3.z << "\n";
                            glWidget->addPoint(v3, panorama->getTranslationVector());

                            outputStream << "v " << v4.x << " " << v4.y << " " << v4.z << "\n";
                            glWidget->addPoint(v4, panorama->getTranslationVector());

                            outputStream << "vt " << x / (width * 1.0f) << " " << (height - y) / (height * 1.0f) << "\n";
                            outputStream << "vt " << (x+1) / (width * 1.0f) << " " << (height - y) / (height * 1.0f) << "\n";
                            outputStream << "vt " << (x+1) / (width * 1.0f) << " " << ((height - y)+1) / (height * 1.0f) << "\n";
                            outputStream << "vt " << x / (width * 1.0f) << " " << ((height - y)+1) / (height * 1.0f) << "\n";

                            //FORMAT: f vertex#/textureCoord#/normal#      *3 = Triangle, *4 = Quad
                            outputStream << "f -4/-4/ -3/-3/ -2/-2/ -1/-1/\n";
                        }

                        float percent = (x * height + y * 1.0f) / (width*height) * 100.0f;

                        emit meshingStatus( percent );

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
            outputStream2 << "newmtl panorama\n"
                          << "Ns 10.0000\n"
                          << "Ni 1.5000\n"
                          << "d 1.0000\n"
                          << "Tr 0.0000\n"
                          << "Tf 1.0000 1.0000 1.0000\n"
                          << "illum 0\n"
                          << "Ka 0.0000 0.0000 0.0000\n"
                          << "Kd 0.5880 0.5880 0.5880\n"
                          << "Ks 0.0000 0.0000 0.0000\n"
                          << "Ke 0.0000 0.0000 0.0000\n"
                          << "map_Kd " << this->panorama->mapFilename << "_colormap.jpg\n";
            file2.close();
        }
        this->meshing = false;
    }

    emit meshingStatus( 100.0f );
    qDebug() << "Mesher just finished!";

    this->deleteLater();

}

void MeshWorker::stopThread()
{
    this->cancelThread = true;
}

