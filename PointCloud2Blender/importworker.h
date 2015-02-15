#ifndef IMPORTWORKER_H
#define IMPORTWORKER_H

#include <QObject>
#include <QRunnable>
#include <QThread>
#include <QDebug>

#include <QFile>

class Point3D
{
    //Example xyz file:
    //-59.43620000 -31.36650000 302.80950000 59 46 55

public:
    //Position Data
    float x,y,z;
    //Color Data
    quint16 r,g,b;
    //Spherical Data
    float alpha, beta, distance;

    QString toString()
    {
        return "(" + QString::number(x) + ", "  + QString::number(y) + ", " + QString::number(z) + ")";
    }
};


class ImportWorker : public QObject, public QRunnable
{
    Q_OBJECT
public:
    enum FileType
    {
        XYZ_ASCII,
        XYZ_BINARY
    };


    explicit ImportWorker(QString fileName, QObject *parent = 0);

    void run();

    FileType fileType;
    QString fileName;


signals:
    void newPoint(Point3D point);
    void importStatus(int percent);

public slots:

};

#endif // IMPORTWORKER_H
