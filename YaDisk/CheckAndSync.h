#ifndef CHECKANDSYNC_H
#define CHECKANDSYNC_H

#include <QObject>
#include <QDir>
#include <QList>
#include <QFileInfo>
//#include <qwebdav/qwebdav.h>


class CheckAndSync : public QObject
{
    Q_OBJECT
public:
    explicit CheckAndSync(QObject *parent = 0);
    QDir HomeDirectory;
    QDir TestDirectory;
    QList<QFileInfo> ListOfFiles;
    QDir NowDirectory;
    bool isOnCloud(QString);

    QDateTime LocalFileLastModifed(QString FileName);
    void Syncronize();

signals:
    void DifferentHash();
    void NeedDownloadFile(QString,QString);
    void NeedUploadFile(QString,QString);
    void NeedFilePropeties(QString,QString);

public slots:
    void FileSync(QString, QString, QString);


};

#endif // CHECKANDSYNC_H
