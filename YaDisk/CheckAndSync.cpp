#include "CheckAndSync.h"
#include <QDebug>
#include <QDateTime>
#include <ClientRequests.h>
#include <QStack>
//#include <qwebdav/qwebdav.h>

CheckAndSync::CheckAndSync(QObject *parent) :
    QObject(parent)
{
    HomeDirectory = "/home/quark/YaDiskDir";
    NowDirectory = "/";
    TestDirectory.setCurrent("/home/quark/YaDiskDir");
    TestDirectory.setFilter(QDir::Files| QDir::Dirs | QDir::Hidden | QDir::NoDot);
    TestDirectory.setSorting(QDir::DirsLast);
    HomeDirectory.setFilter(QDir::Files| QDir::Dirs | QDir::Hidden | QDir::NoDot);
    NowDirectory.setFilter(QDir::Files| QDir::Dirs | QDir::Hidden | QDir::NoDot);
    ListOfFiles = HomeDirectory.entryInfoList();
}

QDateTime CheckAndSync::LocalFileLastModifed(QString FileName){

   QFileInfo *file = new QFileInfo(FileName);
   return file->lastModified();

}

void CheckAndSync::FileSync(QString FilePath,QString FileName,QString FileInfo){

    QString CloudHash = FileInfo.left(32);                                           // из 32-ух символов состоит md5-хэш сумма
    QString *TempDate = new QString(FileInfo.right(29));                             // строка с датой изменения содержит 29 символов

    //*******приведение строки с датой к виду встроенного типа QDateTime*******//
    // изначально выглядит как "Tue, 18 Mar 2014 17:30:08 GMT"
    // а нужно, чтобы было как "Tue Mar 18 17:30:08 2014"
    // т.к. по маске "ddd, dd MMM yyyy hh:mm:ss 'GMT'"  и ей подобным не парсит :(

    TempDate->remove(3,1);
    QString *buffer = new QString;
    *buffer = TempDate->left(6).right(2);
    TempDate->remove(3,3);
    TempDate->insert(7,' '+*buffer);
    *buffer = TempDate->left(15).right(4);
    TempDate->remove(10,5);
    TempDate->insert(20,*buffer);
    TempDate->remove(24,3);

    // TempDate->remove(3,1).remove(6,1).remove(8,1).remove(16,1);
    //**************************************************************************//

    QDateTime *CloudDate = new QDateTime();
    QLocale loc(QLocale::English,QLocale::UnitedStates);
    *CloudDate = loc.toDateTime(*TempDate,"ddd MMM dd hh:mm:ss yyyy");
    delete TempDate;

    qDebug() << QDateTime::currentDateTime().toString("ddd MMM dd hh:mm:ss yyyy")+" | "+"Begin to syncronize "+FilePath+FileName+" ";
    if (CloudHash != ClientRequests::MD5FileSumm(HomeDirectory.path()+FilePath+FileName)){
        if ((*CloudDate) > LocalFileLastModifed(HomeDirectory.path()+FilePath+FileName)){

            emit NeedDownloadFile(FilePath,FileName);
            qDebug() << QDateTime::currentDateTime().toString("ddd MMM dd hh:mm:ss yyyy")+" | "+"NeedDownloadFile signal emitted";
        }
        if ((*CloudDate) < LocalFileLastModifed(HomeDirectory.path()+FilePath+FileName)){

            emit NeedUploadFile(FilePath,FileName);
            qDebug() << QDateTime::currentDateTime().toString("ddd MMM dd hh:mm:ss yyyy")+" | "+"NeedUploadFile signal emitted";
        }
    }
    qDebug() << QDateTime::currentDateTime().toString("ddd MMM dd hh:mm:ss yyyy")+" | "+"File syncronizing complete!";
}

void CheckAndSync::Syncronize(){
    QStack <QFileInfo> File_Stack;
    QFileInfo backdir("..");
    QDir *tempDir = new QDir();
    tempDir->setCurrent("/home/quark/YaDiskDir");
    tempDir->setFilter(QDir::Files| QDir::Dirs | QDir::Hidden | QDir::NoDotAndDotDot);
    tempDir->setSorting(QDir::DirsLast);
    QVector <QString> VizitedDirs;

    qDebug() << QDateTime::currentDateTime().toString("ddd MMM dd hh:mm:ss yyyy")+" | "+"Begin to global syncronizing";
    do {
        if (!VizitedDirs.contains(tempDir->path())) {
            File_Stack.push(backdir);
            for(int i=tempDir->entryInfoList().count();i>0;i--){
                File_Stack.push(tempDir->entryInfoList().at(i-1));

                if (File_Stack.top().isFile()) {
                    if(tempDir->path() == "."){
                        emit NeedFilePropeties("/",File_Stack.pop().fileName());
                        qDebug() << QDateTime::currentDateTime().toString("ddd MMM dd hh:mm:ss yyyy")+" | "+"NeedFileProperties signal emmited";
                    }
                    else {
                        emit NeedFilePropeties('/'+tempDir->path()+'/',File_Stack.pop().fileName());
                        qDebug() << QDateTime::currentDateTime().toString("ddd MMM dd hh:mm:ss yyyy")+" | "+"NeedFileProperties signal emmited";
                    }
                }
            }
        }

        qDebug() << "CD to "+File_Stack.top().fileName();
        VizitedDirs.append(tempDir->path());
        tempDir->cd(File_Stack.pop().fileName());

    }
    while(!File_Stack.empty());
    qDebug() << QDateTime::currentDateTime().toString("ddd MMM dd hh:mm:ss yyyy")+" | "+"Global syncronizing complete.Waiting.";

}
