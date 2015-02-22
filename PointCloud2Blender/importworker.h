#ifndef IMPORTWORKER_H
#define IMPORTWORKER_H

#include <QObject>
#include <QRunnable>
#include <QThread>
#include <QDebug>

#include <QFile>

#include <panorama3d.h>

class Point3D;

class ImportWorker : public QObject, public QRunnable
{
    Q_OBJECT
public:
    enum FileType
    {
        XYZ_ASCII,
        XYZ_BINARY,
        PLY
    };


    explicit ImportWorker(QString fileName, QObject *parent = 0);
    ~ImportWorker();

    void run();
    void import_XYZ_Ascii_File();
    void import_XYZ_Binary_File();
    void import_PLY_File();


    FileType fileType;
    QString fileName;



signals:
    void newPoint(Point3D point);
    void importStatus(int percent);

    void showInfoMessage(QString message);
    void showErrorMessage(QString message);

public slots:

};

#endif // IMPORTWORKER_H
