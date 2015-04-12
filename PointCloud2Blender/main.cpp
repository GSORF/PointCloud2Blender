#include "mainwindow.h"
#include <QApplication>

QString get_string(QString option)
{
    return(option.mid(option.indexOf("=")+1));
}

int get_int(QString option)
{
    return(get_string(option).toInt());
}

float get_float(QString option)
{
    return(get_string(option).toFloat());
}

void usage(QString name)
{
    QString app(name.mid(name.lastIndexOf("/")+1));
    qDebug() << app << "VERSION: 0.1 ALPHA";
    qDebug() << " " << "LICENSE: GPL 3.0";
    qDebug() << " " << "COPYRIGHT: AK 2015";
    qDebug() << " " << "DISCLAIMER: Use this program at your own risk.";
    qDebug() << "usage:";
    qDebug() << " " << app << " {options} {file}";
    qDebug() << "where options are:";
    qDebug() << " --input={file}: your point cloud file";
    qDebug() << " --translation=x,y,z: initial translation of point cloud";
    qDebug() << " --up={left/right}{x/y/z}: coordinate system handedness and up direction";
    qDebug() << " --resolution={1/2/4/8/16}: the resolution of the panorama images";
    qDebug() << " --distance=maxDistance: the maximum distance of a point from the origin in meters";
    qDebug() << " --projection={equirectangular/cylindrical/mercator}: the type of projection you want to use for the panoramas";
    qDebug() << " --nogui: don't show a user interface";
    qDebug() << " --help: this help text";
    qDebug() << "example: .xyz 2 Blender usage";
    qDebug() << " ./" + app + " --input=file.xyz --translation=(20,10,50) --up=leftx --resolution=16 --distance=60 --projection=equirectangular --nogui";
    exit(1);

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setOrganizationName("AK Productions");
    a.setApplicationName("PointCloud2Blender");
    a.setApplicationVersion("0.1");

    //get argument list
    QStringList args = QCoreApplication::arguments();
    QString appname(argv[0]);

    //fill argument and option lists
    QStringList arg, opt;
    for(int i=1; i<args.size(); i++)
    {
        if(args[i].startsWith("--")) opt.push_back(args[i].mid(2));
        else if(args[i].startsWith("-")) opt.push_back(args[i].mid(1));
        else arg.push_back(args[i]);
    }

    //argument/option variables
    QString inputFile="file.xyz";
    QString translation="0,0,0";
    QString up="rightz";
    int resolution=1;
    float distance=60.0f;
    QString projection="equirectangular";
    bool gui=true;

    //Initialize Variables
    for(int i=0; i<opt.size(); i++)
    {
        if(opt[i].startsWith("input=")) inputFile=get_string(opt[i]);
        else if(opt[i].startsWith("translation=")) translation=get_string(opt[i]);
        else if(opt[i].startsWith("up=")) up=get_string(opt[i]);
        else if(opt[i].startsWith("resolution=")) resolution=get_int(opt[i]);
        else if(opt[i].startsWith("distance=")) distance=get_float(opt[i]);
        else if(opt[i].startsWith("projection=")) projection=get_string(opt[i]);
        else if(opt[i] == "nogui") gui=false;
        else if(opt[i] == "help") usage( appname );
        else usage( appname );
    }


    MainWindow w;

    if(!gui)
    {
        //GUI will not start
        w.processCommandLine(inputFile, translation, up, resolution, distance, projection);

    }
    else
    {
        //GUI will start
        w.show();
    }

    return a.exec();
}
