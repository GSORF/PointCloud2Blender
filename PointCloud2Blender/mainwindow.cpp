#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->btnFileOpenDialog, SIGNAL(clicked()), this, SLOT(showFileOpenDialog()));
    connect(ui->btnImport, SIGNAL(clicked()), this, SLOT(startFileImport()));



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showFileOpenDialog()
{
    //The following filetypes are selectable
    QString fileFormat = "XYZ Files (*xyz);;XYZ Binary Files (*xyzb);;All Files (*.*)";
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

    //create a new Thread for importing
    //pass the filename to the importer Thread (Name: ImportWorker)
    //start the thread

    //Later: Connect the importer Thread with the main data container that holds panorama information (or more general: the 3D Point Cloud), (Name: Panorama3D)

    //Everything else happens on the Thread (Importing, maybe calculating spherical coordinates, and sending over the data to the main data container

}
