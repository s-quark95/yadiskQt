#ifndef CLIENT_REQUESTS_H
#define CLIENT_REQUESTS_H
#include <QObject>
#include <QNetworkAccessManager>
#include <QString>
#include <QDebug>
#include <QSslSocket>
#include <QCryptographicHash>
#include <QXmlStreamReader>
#include <QFile>
#include <QDir>
#include <QThread>
#include <QDateTime>
//#include <qwebdav/qwebdav.h>


class ClientRequests: public QObject {
    Q_OBJECT
public:
    explicit ClientRequests(QObject *parent=0);
    ~ClientRequests();
    void SetToken(QByteArray tkn);
    QString FreeAndFeltSpace();
    QString WhatIsHere();
    void CreateDirectory(QString);
    void Delete(QString);
    void Copy(QString, QString);
    void Move(QString, QString);
    static QString MD5FileSumm(QString FullPathAndFileName);
    static QString SHA256Hash(QString FullPathAndFileName);
    QString GetCloudFileMD5(QString FileName);
    QString GetCloudFileLstMfdDate(QString FileName);
signals:
    void SpaceInfoReceived(QString);
    void FileInfoReceived(QString,QString,QString);
private:
    QString ServerAnswer(QByteArray,qint32);
    QByteArray *token;
    QSslSocket *socket;
    QString YaDiskDir;
    QByteArray FinishRequest;
public slots:
    void DownloadFile(QString, QString);
    void UploadFile(QString, QString);
    void ObjPropeties(QString,QString);

};
#endif // CLIENT_REQUESTS_H
