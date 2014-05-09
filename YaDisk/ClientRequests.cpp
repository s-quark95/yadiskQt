#include <ClientRequests.h>
//#include <qwebdav/qwebdav.h>


/*
 ****************************************************************
 *  Здесь содержатся запросы программы на сервер Yandex.Disk
 *  (webdav.yandex.ru)
 *  Также содержится реализация слотов и сигналов.
 *  28.02.2014
 *
 ****************************************************************
*/

ClientRequests::ClientRequests(QObject *parent):QObject(parent){

    token = new QByteArray;
    socket = new QSslSocket;
    YaDiskDir = "/home/quark/YaDiskDir";                     // Директория работы Яндекс.Диска
    FinishRequest = "/r/n/r/n";                              // Строка, завершающая лбой запрос к серверу
    SetToken("yadiskQt:12369874");

    qDebug() << QDateTime::currentDateTime().toString("ddd MMM dd hh:mm:ss yyyy")+" | "+"Initialize connection to webdav.yandex.ru:443";

    socket->connectToHostEncrypted("webdav.yandex.ru", 443); // инициализация сокета, СЕРВЕР:webdav.yandex.ru, ПОРТ: 443

    if (!socket->waitForEncrypted()) {                       // если сервер не ожидает защищенного соединения
       qDebug() << QDateTime::currentDateTime().toString("ddd MMM dd hh:mm:ss yyyy")+" | "+socket->errorString();                    // выводим в дебаг причину сего
    }
    else
       qDebug() << QDateTime::currentDateTime().toString("ddd MMM dd hh:mm:ss yyyy")+" | "+"Connection initialized!";

}

ClientRequests::~ClientRequests()
{
    delete token;
    delete socket;
}

//-----------------------------------------------------
// "шифрование" токена
//-----------------------------------------------------
void ClientRequests::SetToken(QByteArray tkn)
{
    token->append(tkn);                                       // инициализация токена и
    *token = token->toBase64();                               // конвертация в base64 кодировку
}

//----------------------------------------------------
// реализация метода чтения ответа от сервера
//----------------------------------------------------
QString ClientRequests::ServerAnswer(QByteArray RequestToServer,qint32 timeout)
{
    QString Output;

    qDebug() << QDateTime::currentDateTime().toString("ddd MMM dd hh:mm:ss yyyy")+" | "+"Sending request to socket";
    socket->write(RequestToServer);

    while (socket->waitForReadyRead(timeout))                  // пока можно читать
       Output.append(socket->readAll());                       // читаем данные от сервера
    qDebug() << QDateTime::currentDateTime().toString("ddd MMM dd hh:mm:ss yyyy")+" | "+"Server's answer received";
    return Output;
}

//----------------------------------------------------
// метод подсчета md5-суммы у файла
//----------------------------------------------------
QString ClientRequests::MD5FileSumm(QString FullPathAndFileName)
{
    QFile *File = new QFile(FullPathAndFileName);

    if (!File->exists())                                        // Если файл не существует, то руагемся!
       return "There is no such file!";
    qDebug() << QDateTime::currentDateTime().toString("ddd MMM dd hh:mm:ss yyyy")+" | "+"Calculating localfile "+File->fileName()+" md5-hash summ";

    File->open(QIODevice::ReadOnly);
    QCryptographicHash *Local = new QCryptographicHash(QCryptographicHash::Md5);

    while (!File->atEnd())
       Local->addData(File->read(65536));                       // читаем файл чанками по 64K

    File->close();
    delete File;

    qDebug() << QDateTime::currentDateTime().toString("ddd MMM dd hh:mm:ss yyyy")+" | "+"Calculating md5-hash summ complete!";
    return Local->result().toHex();

}

//----------------------------------------------------
// метод подсчета SHA-256 хэша у файла
//----------------------------------------------------
QString ClientRequests::SHA256Hash(QString FullPathAndFileName)
{
    QFile *File = new QFile(FullPathAndFileName);

    if(!File->exists())
       return "There is no such file!";

    qDebug() << QDateTime::currentDateTime().toString("ddd MMM dd hh:mm:ss yyyy")+" | "+"Calculating localfile "+File->fileName()+" SHA-256 hash";
    File->open(QIODevice::ReadOnly);
    QCryptographicHash *ShaHash = new QCryptographicHash(QCryptographicHash::Sha256);

    while (!File->atEnd())
       ShaHash->addData(File->read(65546));

    File->close();
    delete File;

    qDebug() << QDateTime::currentDateTime().toString("ddd MMM dd hh:mm:ss yyyy")+" | "+"Calculating SHA-256 complete!";
    return ShaHash->result().toHex();

}

//----------------------------------------------------
// запрос на md5-сумму файла на сервере
//----------------------------------------------------
QString ClientRequests::GetCloudFileMD5(QString FileName)
{
    /*----------Формирование запроса----------*/
    QByteArray *Request = new QByteArray("PROPFIND ");
    Request->append(FileName);
    Request->append(" HTTP/1.1\nHost: webdav.yandex.ru\nAccept: */*\nDepth: 1\nAuthorization: Basic ");
    Request->append(*token);
    Request->append("\r\n\r\n");
    /*----------------------------------------*/

    QString CloudFileMD5,reply;

    qDebug() << QDateTime::currentDateTime().toString("ddd MMM dd hh:mm:ss yyyy")+" | "+"Getting md5-hash "+FileName+" from cloud";

    reply = ServerAnswer(*Request,5000);
    delete Request;
    if (reply.contains("Not found") || reply.contains("Bad request") || reply.contains("Internal Server Error")) {
        qDebug() << QDateTime::currentDateTime().toString("ddd MMM dd hh:mm:ss yyyy")+" | "+"Something wrong: "+reply;
       return "000000000000000000000000000000000";
    }

    qint32 index = reply.indexOf("<d:getetag>") + 11;
    while (reply.at(index) != '<') {
        CloudFileMD5.append(reply.at(index));
        index++;
    }

    qDebug() << QDateTime::currentDateTime().toString("ddd MMM dd hh:mm:ss yyyy")+" | "+"Getting complete!";
    return CloudFileMD5;
}

//----------------------------------------------------
// запрос на дату последеного изменения файла на сервере
//----------------------------------------------------
QString ClientRequests::GetCloudFileLstMfdDate(QString FileName)
{
    /*----------Формирование запроса----------*/
    QByteArray *Request = new QByteArray("PROPFIND ");
    Request->append(FileName);
    Request->append(" HTTP/1.1\nHost: webdav.yandex.ru\nAccept: */*\nDepth: 1\nAuthorization: Basic ");
    Request->append(*token);
    Request->append("\r\n\r\n");
    /*----------------------------------------*/

    QString CloudFileLastModifed,reply;
    qDebug() << QDateTime::currentDateTime().toString("ddd MMM dd hh:mm:ss yyyy")+" | "+"Getting last modifed date "+FileName+" from cloud";
    reply = ServerAnswer(*Request,5000);
    delete Request;

    // qDebug() << reply;
    if (reply.contains("Not found") || reply.contains("Bad request") || reply.contains("Internal Server Error")) {
        qDebug() << QDateTime::currentDateTime().toString("ddd MMM dd hh:mm:ss yyyy")+" | "+"Something wrong: "+reply;
        return "Sun, 01 Jan 1900 00:00:00 GMT";                 // Возвращаем очень старую дату
    }


    qint32 index = reply.indexOf("<d:getlastmodified>") + 19;
    while (reply.at(index) != '<') {
        CloudFileLastModifed.append(reply.at(index));
        index++;
    }

    qDebug() << QDateTime::currentDateTime().toString("ddd MMM dd hh:mm:ss yyyy")+" | "+"Getting complete!";
    return CloudFileLastModifed;
}

//!================================================ПОЛУЧЕНИЕ СВОЙСТВ ФАЙЛОВ И КАТАЛОГОВ====================================================
//------------------------------------------------------
// запрос на свободное и занятое место на YDisk клиента
//------------------------------------------------------
QString ClientRequests::FreeAndFeltSpace()
{
    // формирование текста запроса
    QByteArray PROFING_req("PROPFIND / HTTP/1.1\nHost: webdav.yandex.ru\nAccept: */*\nDepth: 0\nAuthorization: Basic ");
    PROFING_req.append(*token);
    PROFING_req.append("\nContent-Length: 143\nContent-type: application/xml; charset=\"utf-8\"\n");
    PROFING_req.append("\n<?xml version=\"1.0\" encoding=\"utf-8\" ?><D:propfind xmlns:D=\"DAV:\"><D:prop><D:quota-available-bytes/><D:quota-used-bytes/></D:prop></D:propfind>");
    PROFING_req.append("\r\n\r\n");

    //Парсинг ответа
    QString TMP = ServerAnswer(PROFING_req,3000);
    QString Output;

    if (TMP.indexOf("HTTP/1.1 207")!= -1) {
        qint32 inputIndex = TMP.indexOf("<d:quota-used-bytes>",170); // позиция в строке с ответом, с которой начинается <d:quota-....
        inputIndex+=20;                                         // так как, в самом теге 20 символов, то еще немножко смещаем

        while (TMP.at(inputIndex)!='<') {                       // пока тег не закрыт, читаем символы
          Output.append( TMP.at(inputIndex));
         inputIndex++;
        }

        Output.append(";");                                     // добавляем разделительный символ

        inputIndex = TMP.indexOf("<d:quota-available-bytes>",170);   // проделываем аналогичную операцию и для парсинга доступного места на диске
        inputIndex +=25;
        while (TMP.at(inputIndex)!='<') {
          Output.append( TMP.at(inputIndex));
        inputIndex++;
        }
    }
    else
        return "*ERROR_free&felt*";
   emit SpaceInfoReceived(Output);
   return Output;
}


//------------------------------------------------------
// запрос на отображение содержимого Yandex.Disk
//------------------------------------------------------
QString ClientRequests::WhatIsHere()
{
    QByteArray PROF_req("PROPFIND / HTTP/1.1\nHost: webdav.yandex.ru\nAccept: */*\nDepth: 1\nAuthorization: Basic ");
    PROF_req.append(*token);
    PROF_req.append("\n\r\n\r\n");

    return ServerAnswer(PROF_req,5000);
}

//-----------------------------------------------------
// Запрос на свойство файла FileName
//-----------------------------------------------------
void ClientRequests::ObjPropeties(QString Path,QString FileName)
{
    QString *resp = new QString();

    qDebug() << QDateTime::currentDateTime().toString("ddd MMM dd hh:mm:ss yyyy")+" | "+"Begin asking "+Path+FileName+" properties";
    *resp = GetCloudFileMD5(Path+FileName);
    resp->append("\n") += GetCloudFileLstMfdDate(Path+FileName);

    qDebug() << QDateTime::currentDateTime().toString("ddd MMM dd hh:mm:ss yyyy")+" | "+"Asking complete! FileInfoReceived signal emitted";

    emit FileInfoReceived(Path,FileName,*resp);
    delete resp;
}

//!================================================ОПЕРАЦИИ С ФАЙЛАМИ И КАТАЛОГАМИ==========================================================

/*********************************************************************************
 *                  Скачивание файла с Yandex.Disk'a
 * Скачивание производиться также чанками по 64КБ.
 * Если скачиваемый файл существует на локальном диске, то он будет удален.
 * ДОПИЛИТЬ: Сделать нормальную докачку после краша соединения или программы
 * и проверку на ошибочные ответы от сервера.
 *
 *********************************************************************************
 */

void ClientRequests::DownloadFile(QString Path,QString FileName)
{

    ////////////////////////////////// Формирование запроса ///////////////////////////////////
      QByteArray *Request = new QByteArray("GET ");
      Request->append(Path+FileName);
      Request->append(" HTTP/1.1\nHost: webdav.yandex.ru\nAccept: */*\nAuthorization: Basic ");
      Request->append(*token);
      Request->append("\r\n\r\n");
    ///////////////////////////////////////////////////////////////////////////////////////////

    socket->write(*Request);
    delete Request;

    QDir *Dir = new QDir(Path);
    if (!Dir->exists())                                         // Если такой директории не существует,
       Dir->mkpath(Dir->path());                                // то создаем ее

    QFile *DownloadedFile = new QFile(Dir->path()+FileName);

    if (DownloadedFile->exists())                               // Если файл существует - он будет удален
        DownloadedFile->remove();
    qDebug() << QDateTime::currentDateTime().toString("ddd MMM dd hh:mm:ss yyyy")+" | "+"Begin downloading file "+DownloadedFile->fileName()+" ("+DownloadedFile->size()+") bytes";

    DownloadedFile->open(QIODevice::ReadWrite);
    while (socket->waitForReadyRead())                          // Скачка
       DownloadedFile->write(socket->read(65536));              // чанками по 64К за итерацию

    DownloadedFile->flush();
    DownloadedFile->close();

    delete Dir;
    delete DownloadedFile;

    qDebug() << QDateTime::currentDateTime().toString("ddd MMM dd hh:mm:ss yyyy")+" | "+"Download complete!";
}

/*********************************************************************************
 *                  Заливка файла на Yandex.Disk
 * Заливка файла осуществляется двумя направлениями:
 * 1) Если размер файла меньше либо равен 64 КБ, то он считывается
 *    полностью в оперативную память и далее вставляется в непосредственно
 *    в сам запрос.
 * 2) Если размер файла превышает 64КБ, то он заливается чанками по 64 КБ.
 *
 * ДОПИЛИТЬ: при загрузке чанками, вне зависимости от размера файла (100МБ-1.5ГБ)
 *          программа начинает "съедать" около 800МБ оперативной памяти.
 *********************************************************************************
 */
void ClientRequests::UploadFile(QString Path,QString FileName)
{
    /////////////////////////////// Формирование запроса //////////////////////////////////////
    QByteArray *Request = new QByteArray("PUT ");
    Request->append(Path+FileName);
    Request->append(" HTTP/1.1\nHost: webdav.yandex.ru\nAccept: */*\nAuthorization: Basic ");
    Request->append(*token);
    Request->append("\nEtag: ");
    Request->append(MD5FileSumm(YaDiskDir+Path+FileName));
    Request->append("\nSha256: ");
    Request->append(SHA256Hash(YaDiskDir+Path+FileName));
    Request->append("\nExpect: 100-continue\nContent-Type: application/binary\n");
    //////////////////////////////////////////////////////////////////////////////////////////


    QFile *File = new QFile(YaDiskDir+Path+FileName);

    qDebug() << QDateTime::currentDateTime().toString("ddd MMM dd hh:mm:ss yyyy")+" | "+"Begin uploading file "+Path+FileName+" ("+QByteArray::number(File->size())+") bytes";
    File->open(QIODevice::ReadOnly);

    // Проверка на размер файла
    // Если его размер меньше либо равен 64КБ
  //  if (File->size() <= 65536) {
        // Если на сервере данного файла нет, он ответит кодом "100 Continue", тем самым разрешая его заливку
        if (ServerAnswer(*Request+"Content-Length: "+QByteArray::number(File->size())+"\r\n\r\n",10000).contains("HTTP/1.1 100 Continue")) {
           //Отсылаем серверу данные
           ServerAnswer(File->readAll(),10000);
        }
   /* }
    else
        // Тоже, что и выше, только для случая с бОльшим файлом
        if (ServerAnswer(Request->append("Content-Length: "+QByteArray::number(File->size()))+"\r\n\r\n",10000).contains("HTTP/1.1 100 Continue")) {
           // Черт знает что, по приколу написал :)
           // socket->setSocketOption(QAbstractSocket::KeepAliveOption,1);
           // Пока есть соединение с сервером и файл не прочитан
           while (!File->atEnd()) {
               // Сначала думал что проблема с потреблением ОП заключается в статичной природе tmparray
               // затем сделал динамичное выделение памяти, но не помогло :( (socket->state() == QSslSocket::ConnectedState) &&
                socket->write(File->read(262144));
                //QThread::msleep(300);
           }
           // Отсылка запроса
           socket->write(FinishRequest);
           /*QWebdav *webby = new QWebdav;
           webby->connectToHostEncrypted("webdav.yandex.ru",443);
           webby->setConnectionSettings(QWebdav::HTTPS,"webdav.yandex.ru","/","yadiskQt","12369874");

           webby->put(Path+FileName,File);*/


    //    }
    File->close();
    qDebug() << QDateTime::currentDateTime().toString("ddd MMM dd hh:mm:ss yyyy")+" | "+"Uploading complete!";

    delete File;
    delete Request;
}

//---------------------------------------------------
//  Создание каталога на Yandex.Disk
//---------------------------------------------------
void ClientRequests::CreateDirectory(QString Directory)
{
    QByteArray mkDir_req("MKCOL ");
    mkDir_req.append(Directory);
    mkDir_req.append(" HTTP/1.1\nHost: webdav.yandex.ru\nAccept: */*\nAuthorization: Basic ");
    mkDir_req.append(*token);
    mkDir_req.append("\r\n\r\n");

    ServerAnswer(mkDir_req,3000);
}

//---------------------------------------------------
//  Удаление каталога или файла на Yandex.Disk
//---------------------------------------------------
void ClientRequests::Delete(QString FileOrDir)
{
    QByteArray dlDir_req("DELETE ");
    dlDir_req.append(FileOrDir);
    dlDir_req.append(" HTTP/1.1\nHost: webdav.yandex.ru\nAccept: */*\nAuthorization: Basic ");
    dlDir_req.append(*token);
    dlDir_req.append("\r\n\r\n");

    ServerAnswer(dlDir_req,5000);

}

//---------------------------------------------------
//  Копирование каталога или файла на Yandex.Disk
//---------------------------------------------------
//! почему-то не копирует файл в директорию, хз шо це таке
void ClientRequests::Copy(QString Source, QString Destination)
{
    QByteArray Cp_req("COPY ");
    Cp_req.append(Source);
    Cp_req.append(" HTTP/1.1\nHost: webdav.yandex.ru\nAccept: */*\nAuthorization: Basic ");
    Cp_req.append(*token);
    Cp_req.append("\nDestination: ");
    Cp_req.append(Destination);
    Cp_req.append("\r\n\r\n");

    ServerAnswer(Cp_req,5000);
}

//---------------------------------------------------
//  Перемещение каталога или файла на Yandex.Disk'е
//---------------------------------------------------
void ClientRequests::Move(QString Source, QString Destination)
{
    QByteArray Cp_req("MOVE ");
    Cp_req.append(Source);
    Cp_req.append(" HTTP/1.1\nHost: webdav.yandex.ru\nAccept: */*\nAuthorization: Basic ");
    Cp_req.append(*token);
    Cp_req.append("\nDestination: ");
    Cp_req.append(Destination);
    Cp_req.append("\r\n\r\n");

    ServerAnswer(Cp_req,5000);
}



