#include "mainwindow.h"
#include "ClientRequests.h"
#include "CheckAndSync.h"
#include "TrayIcon.h"
#include <QApplication>
#include <QMenu>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ClientRequests client;
    CheckAndSync checker;
    QObject::connect(&client,SIGNAL(FileInfoReceived(QString,QString,QString)),&checker,SLOT(FileSync(QString,QString,QString)));
    QObject::connect(&checker,SIGNAL(NeedDownloadFile(QString,QString)),&client,SLOT(DownloadFile(QString,QString)));
    QObject::connect(&checker,SIGNAL(NeedUploadFile(QString,QString)),&client,SLOT(UploadFile(QString,QString)));
    QObject::connect(&checker,SIGNAL(NeedFilePropeties(QString,QString)),&client,SLOT(ObjPropeties(QString,QString)));
    TrayIcon iconka;
    QSize sizer;
    QIcon tempIcon;
    QMenu menushka;
    menushka.addAction("Exit!");
    sizer.setHeight(32);
    sizer.setWidth(32);
    tempIcon.addFile("/home/quark/iconka.png",sizer,tempIcon.Normal,tempIcon.Off);
    iconka.setIcon(tempIcon);
    iconka.setContextMenu(&menushka);
    iconka.show();
    checker.Syncronize();
    return a.exec();
}
