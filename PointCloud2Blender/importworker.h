#ifndef IMPORTWORKER_H
#define IMPORTWORKER_H

#include <QObject>
#include <QRunnable>
#include <QThread>
#include <QDebug>

#include <QVector3D>

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
    void newPoint(QVector3D point);
    void importStatus(int percent);

public slots:

};

#endif // IMPORTWORKER_H
