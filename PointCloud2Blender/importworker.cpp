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

    //send the current data set over to the panorama data container

    emit newPoint( QVector3D(1,12,3) );

    emit importStatus(10);



    //Everything else happens on the Thread (Importing, maybe calculating spherical coordinates, and sending over the data to the main data container




    //after importing send a finished signal?
    emit importStatus(100);

}
