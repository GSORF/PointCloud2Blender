/*
    PointCloud2Blender - convert point cloud files to a textured 3D mesh
    Copyright (C) 2015 Adam Kalisz (Bachelor@Kalisz.co)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QDebug>

#include <QFileDialog>
#include <QMessageBox>

#include <QVector3D>

#include <QSettings>

#include <QThreadPool>
#include "importworker.h"
#include "panorama3d.h"
#include "meshworker.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    ImportWorker *importer;
    Panorama3D *panorama;
    MeshWorker *mesher;
    QThreadPool threadPool;

    Panorama3D::Orientation orientation;
    quint8 resolution;
    int originalHorizontalResolution;
    int originalVerticalResolution;
    bool analyzingOriginalResolution;
    int customPanoramaWidth;
    int customPanoramaHeight;
    QVector3D translation;
    Panorama3D::ProjectionType projectionType;
    float maxDistance;

    QSettings settings;
    qint64 startTime;

    QMenu *menuFile;
    QAction *actOpen;
    QMenu *menuOpenRecent;
    QAction *actOpenRecent;
    QAction *actExit;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void processCommandLine(QString inputFile, QString translation, QString up, int resolution, float distance, QString projection);

private:
    Ui::MainWindow *ui;

    void generateMenus();
    void calculateCustomResolution(int horizontal, int vertical, int divisor);

public slots:
    void showFileOpenDialog();
    void setFilePath(QString fileName);
    void startFileImport();
    void openRecentFile();
    void updateImportStatus(float percent);
    void updateMeshingStatus(float percent);
    void setOriginalResolution(int horizontalResolution);

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
    void onChangeMaxDistance(double distance);
    void onClickProjectionTypeEquirectangular();
    void onClickProjectionTypeCylindrical();
    void onClickProjectionTypeMercator();

    void onChangeResolutionHorizontal(int value);
    void onChangeResolutionVertical(int value);
    void onChangeResolutionDivisor(int value);
    void onClickPanoramaResolutionCustom();
    void onClickDeterminePanoramaResolution();
    void onChangeImportPath(QString path);



    //Callbacks for Panorama Export:
    void onClickExportPanoramas();
    void onClickExportMesh();

    void showInfoMessage(QString message);
    void showErrorMessage(QString message);
};

#endif // MAINWINDOW_H
