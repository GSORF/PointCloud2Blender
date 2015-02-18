#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("AK Productions");
    a.setApplicationName("PointCloud2Blender");
    MainWindow w;
    w.show();

    return a.exec();
}
