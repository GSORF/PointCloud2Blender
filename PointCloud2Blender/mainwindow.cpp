#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    importer = NULL;

    connect(ui->btnFileOpenDialog, SIGNAL(clicked()), this, SLOT(showFileOpenDialog()));
    connect(ui->btnImport, SIGNAL(clicked()), this, SLOT(startFileImport()));



}

MainWindow::~MainWindow()
{
    delete ui;
    delete importer;
    importer = NULL;
}

void MainWindow::showFileOpenDialog()
{
    //The following filetypes are selectable
    QString fileFormat = "XYZ Files (*xyz);;XYZ Binary Files (*xyb);;All Files (*.*)";
    QString fileName = "";

    fileName = QFileDialog::getOpenFileName(this, "Please specify your point cloud file", QDir::homePath(), fileFormat);

    if(!fileName.isEmpty())
    {
        ui->txtFilePathImport->setText(fileName);
        ui->btnImport->setEnabled(true);
    }
    else
    {
        ui->btnImport->setEnabled(false);
    }
}

void MainWindow::startFileImport()
{
    qDebug() << "MainWindow::startFileImport()";

    importer = new ImportWorker(ui->txtFilePathImport->text());
    //Later: Connect the importer Thread with the main data container that holds panorama information (or more general: the 3D Point Cloud), (Name: Panorama3D)
    //connect(importer, SIGNAL(newPoint(QVector3D)), PANORAMAOBJECT, SLOT(...)
    connect(importer, SIGNAL(importStatus(int)), SLOT(updateImportStatus(int)));
    threadPool.start(importer);
}

void MainWindow::updateImportStatus(int percent)
{
    ui->prbImportStatus->setValue(percent);

    if(percent >= 100)
    {
        ui->prbImportStatus->setValue(0);
    }
}
