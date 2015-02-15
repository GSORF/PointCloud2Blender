#include "importworker.h"

ImportWorker::ImportWorker(QString fileName, QObject *parent) :
    QObject(parent)
{
    //Take the filename and determine the filetype (in the beginning just .xyz)

    this->fileName = fileName;
    if(this->fileName.endsWith(".xyz"))
    {
        this->fileType = XYZ_ASCII;
    }
    else if(this->fileName.endsWith(".xyb"))
    {
        this->fileType = XYZ_BINARY;
    }

}

void ImportWorker::run()
{
    qDebug() << "Thread " << QThread::currentThread() << " is running";

    //import the filename
    qDebug() << "opening file: " << this->fileName;

    QFile file(this->fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream inputStream(&file);
    QString line = inputStream.readLine();
    qDebug() << "line before loop" << line;
    while(!line.isNull())
    {
        //BUG: First Line is not being imported...
        qDebug() << "line inside loop" << line;


        Point3D _newPoint;

        //get the contents from the TextStreamObject
        inputStream >> _newPoint.x >> _newPoint.y >> _newPoint.z >>
                       _newPoint.r >> _newPoint.g >> _newPoint.b;


        //send the current point over to the panorama data container
        emit newPoint( _newPoint );

        qDebug() << _newPoint.toString();

        emit importStatus(10);

        //read the line
        line = inputStream.readLine();
    }






    //Everything else happens on the Thread (Importing, maybe calculating spherical coordinates, and sending over the data to the main data container




    //after importing send a finished signal?
    emit importStatus(100);

}
