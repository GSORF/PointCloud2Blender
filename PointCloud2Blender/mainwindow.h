#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QDebug>

#include <QFileDialog>

#include <QThreadPool>
#include <importworker.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    ImportWorker *importer;
    QThreadPool threadPool;


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public slots:
    void showFileOpenDialog();
    void startFileImport();
    void updateImportStatus(int percent);
};

#endif // MAINWINDOW_H
