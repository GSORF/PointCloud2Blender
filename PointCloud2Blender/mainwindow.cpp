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

    generateMenus();
}

MainWindow::~MainWindow()
{
    delete ui;
    importer = NULL;
    delete importer;

    //Note: Delete menus?
}

void MainWindow::generateMenus()
{
    //Load application settings
    QString recentPath = settings.value("paths/recentPath", "").toString();

    //Generate menus
    menuFile = new QMenu("&File", this);
    actOpen = new QAction("&Open...", this);
    actOpen->setShortcut(QKeySequence::Open);
    actOpen->setStatusTip( tr("Open a new point cloud file") );
    connect(actOpen, SIGNAL(triggered()), this, SLOT(showFileOpenDialog()));
    if(!recentPath.isEmpty())
    {
        menuOpenRecent = new QMenu("Open &recent...", menuFile);
        actOpenRecent = new QAction(recentPath, this);
        connect(actOpenRecent, SIGNAL(triggered()), this, SLOT(openRecentFile()));
    }
    actExit  = new QAction("&Exit", this);
    connect(actExit, SIGNAL(triggered()), this, SLOT(close()));

    //Add menus
    ui->menuBar->addMenu(menuFile);
    menuFile->addAction(actOpen);
    if(!recentPath.isEmpty())
    {
        menuFile->addMenu(menuOpenRecent);
        menuOpenRecent->addAction(actOpenRecent);
    }
    menuFile->addSeparator();
    menuFile->addAction(actExit);

}

void MainWindow::showFileOpenDialog()
{
    //The following filetypes are selectable
    QString fileFormat = "XYZ Files (*xyz);;XYZ Binary Files (*xyb);;All Files (*.*)";
    QString fileName = "";

    fileName = QFileDialog::getOpenFileName(this, "Please specify your point cloud file", QDir::homePath(), fileFormat);

    setFilePath(fileName);
}

void MainWindow::setFilePath(QString fileName)
{
    //Save recent Path as setting
    settings.setValue("paths/recentPath", fileName);

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
    connect(importer, SIGNAL(importStatus(int)), this, SLOT(updateImportStatus(int)));
    connect(importer, SIGNAL(showInfoMessage(QString)), this, SLOT(showInfoMessage(QString)));
    connect(importer, SIGNAL(showErrorMessage(QString)), this, SLOT(showErrorMessage(QString)));

    threadPool.start(importer);
}

void MainWindow::openRecentFile()
{
    //Get the text from the menu item and use it as the filepath
    QAction *action = qobject_cast<QAction*>(sender());
    if(action)
    {
        setFilePath(action->text());
        startFileImport();
    }
}

void MainWindow::updateImportStatus(int percent)
{
    ui->prbImportStatus->setValue(percent);

    if(percent >= 100)
    {
        ui->prbImportStatus->setValue(0);
    }
}

void MainWindow::showInfoMessage(QString message)
{
    QMessageBox::information(this, "Information", message);
}

void MainWindow::showErrorMessage(QString message)
{
    QMessageBox::critical(this, "Error", message);
}
