#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QDebug>

#include <QFileDialog>
#include <QMessageBox>

#include <QSettings>

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

    QSettings settings;

    QMenu *menuFile;
    QAction *actOpen;
    QMenu *menuOpenRecent;
    QAction *actOpenRecent;
    QAction *actExit;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    void generateMenus();

public slots:
    void showFileOpenDialog();
    void setFilePath(QString fileName);
    void startFileImport();
    void openRecentFile();
    void updateImportStatus(int percent);

    void showInfoMessage(QString message);
    void showErrorMessage(QString message);
};

#endif // MAINWINDOW_H
