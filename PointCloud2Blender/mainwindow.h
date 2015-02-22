#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QDebug>

#include <QFileDialog>
#include <QMessageBox>

#include <QVector3D>

#include <QSettings>

#include <QThreadPool>
#include <importworker.h>
#include <panorama3d.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    ImportWorker *importer;
    Panorama3D *panorama;
    QThreadPool threadPool;

    Panorama3D::Orientation orientation;
    quint8 resolution;
    QVector3D translation;

    QSettings settings;

    QMenu *menuFile;
    QAction *actOpen;
    QMenu *menuOpenRecent;
    QAction *actOpenRecent;
    QAction *actExit;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void processCommandLine(QString inputFile, QString translation, QString up, int resolution);

private:
    Ui::MainWindow *ui;

    void generateMenus();

public slots:
    void showFileOpenDialog();
    void setFilePath(QString fileName);
    void startFileImport();
    void openRecentFile();
    void updateImportStatus(int percent);

    void updateDepthMap(QImage *depthMap);
    void updateColorMap(QImage *colorMap);


    //Callbacks for Panorama settings:
    void onClickUpVectorLeftX();
    void onClickUpVectorLeftY();
    void onClickUpVectorLeftZ();
    void onClickUpVectorRightX();
    void onClickUpVectorRightY();
    void onClickUpVectorRightZ();
    void onClickPanoramaResolutionX1();
    void onClickPanoramaResolutionX2();
    void onClickPanoramaResolutionX4();
    void onClickPanoramaResolutionX8();
    void onClickPanoramaResolutionX16();
    void onChangeTranslationVectorX(double x);
    void onChangeTranslationVectorY(double y);
    void onChangeTranslationVectorZ(double z);

    //Callbacks for Panorama Export:
    void onClickExportPanoramas();
    void onClickExportMesh();

    void showInfoMessage(QString message);
    void showErrorMessage(QString message);
};

#endif // MAINWINDOW_H
