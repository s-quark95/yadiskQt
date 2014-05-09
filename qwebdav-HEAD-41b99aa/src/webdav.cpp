/* This file is part of QWebdav
 *
 * Copyright (C) 2009-2010 Corentin Chary <corentin.chary@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include <QDomDocument>
#include <QDomNode>
#include <QUrl>
#include <QDebug>

#include "webdav.h"
#include "webdav_url_info.h"

QWebdav::QWebdav (QObject *parent)
  : QHttp(parent)
{
  init("");
}

QWebdav::QWebdav (const QString &hostName, quint16 port, QObject *parent)
  : QHttp(hostName, port, parent)
{
  init(hostName);
}

QWebdav::QWebdav ( const QString & hostName, ConnectionMode mode, quint16 port,
		   QObject * parent)
  : QHttp(hostName, mode, port, parent)
{
  init(hostName);
}

QWebdav::~QWebdav ()
{
}

int
QWebdav::setHost ( const QString & hostName, quint16 port )
{
  host = hostName;
  return QHttp::setHost(hostName, port);
}

int
QWebdav::setHost ( const QString & hostName, ConnectionMode mode, quint16 port)
{
  host = hostName;
  return QHttp::setHost(hostName, mode, port);
}

void
QWebdav::init(const QString & hostName)
{
  host = hostName;
  emitListInfo = false;
  connect(this, SIGNAL(readyRead ( const QHttpResponseHeader & )),
	  this, SLOT(readyRead ( const QHttpResponseHeader & )));
  connect(this, SIGNAL(requestFinished ( int, bool )),
	  this, SLOT(requestFinished ( int, bool )));
  connect(this, SIGNAL(responseHeaderReceived( const QHttpResponseHeader & )),
	  this, SLOT(responseHeaderReceived( const QHttpResponseHeader & )));
}

int
QWebdav::list ( const QString & dir)
{
  QWebdav::PropNames query;
  QStringList props;

  props << "creationdate";
  props << "getcontentlength";
  props << "displayname";
  props << "source";
  props << "getcontentlanguage";
  props << "getcontenttype";
  props << "executable";
  props << "getlastmodified";
  props << "getetag";
  props << "resourcetype";

  query["DAV:"] = props;

  return propfind(dir, query, 1);
}

int
QWebdav::search ( const QString & path, const QString & q )
{
  QByteArray query = "<?xml version=\"1.0\"?>\r\n";
  QString dir = path;

  query.append( "<D:searchrequest xmlns:D=\"DAV:\">\r\n" );
  query.append( q.toUtf8() );
  query.append( "</D:searchrequest>\r\n" );

  QHttpRequestHeader req("SEARCH", path);
  return davRequest(req);
}

int
QWebdav::put ( const QString & path, QIODevice * data )
{
  QHttpRequestHeader req("PUT", path);

  return davRequest(req, data);
}

int
QWebdav::put ( const QString & path, const QByteArray & data )
{
  QHttpRequestHeader req("PUT", path);

  return davRequest(req, data);
}

int
QWebdav::propfind ( const QString & path, const QWebdav::PropNames & props,
		    int depth)
{
  QByteArray query;

  query = "<?xml version=\"1.0\" encoding=\"utf-8\" ?>";
  query += "<D:propfind xmlns:D=\"DAV:\" >";
  query += "<D:prop>";
  foreach (QString ns, props.keys())
    {
      foreach (const QString key, props[ns])
	if (ns == "DAV:")
	  query += "<D:" + key + "/>";
	else
      	  query += "<" + key + " xmlns=\"" + ns + "\"/>";
    }
  query += "</D:prop>";
  query += "</D:propfind>";
  return propfind(path, query, depth);
}


int
QWebdav::propfind( const QString & path, const QByteArray & query, int depth )
{
  QHttpRequestHeader req("PROPFIND", path);
  QString value;

  if (depth == 2)
    value = "infinity";
  else
    value = QString("%1").arg(depth);
  req.setValue("Depth", value);
  return davRequest(req, query);
}

int
QWebdav::proppatch ( const QString & path, const QWebdav::PropValues & props)
{
  QByteArray query;

  query = "<?xml version=\"1.0\" encoding=\"utf-8\" ?>";
  query += "<D:proppatch xmlns:D=\"DAV:\" >";
  query += "<D:prop>";
  foreach (QString ns, props.keys())
    {
      QMap < QString , QVariant >::const_iterator i;

      for (i = props[ns].constBegin(); i != props[ns].constEnd(); ++i) {
	if (ns == "DAV:") {
	  query += "<D:" + i.key() + ">";
	  query += i.value().toString();
	  query += "</D:" + i.key() + ">" ;
	} else {
	  query += "<" + i.key() + " xmlns=\"" + ns + "\">";
	  query += i.value().toString();
	  query += "</" + i.key() + " xmlns=\"" + ns + "\"/>";
	}
      }
    }
  query += "</D:prop>";
  query += "</D:propfind>";

  return proppatch(path, query);
}

int
QWebdav::proppatch( const QString & path, const QByteArray & query)
{
  QHttpRequestHeader req("PROPPATCH", path);

  return davRequest(req, query);
}

void
QWebdav::readyRead ( const QHttpResponseHeader & rep )
{
  if (emitListInfo)
    buffer += readAll();
}

void
QWebdav::emitListInfos()
{
  QDomDocument multiResponse;
  bool hasResponse = false;

  multiResponse.setContent(buffer, true);

  for ( QDomNode n = multiResponse.documentElement().firstChild();
        !n.isNull(); n = n.nextSibling())
    {
      QDomElement thisResponse = n.toElement();

      if (thisResponse.isNull())
	continue;

      QWebdavUrlInfo info(thisResponse);

      if (!info.isValid())
	continue;

      hasResponse = true;
      emit listInfo(info);
    }
}

void
QWebdav::responseHeaderReceived( const QHttpResponseHeader & resp )
{
  QHttpRequestHeader req = currentRequest();
  QString method = req.method().toUpper();

  if (method == "PROPFIND" || method == "SEARCH")
    emitListInfo = true;
}

void
QWebdav::requestFinished ( int id, bool error )
{
  if (emitListInfo && !error)
    emitListInfos();
  buffer.clear();
  emitListInfo = false;
}

void
QWebdav::setupHeaders(QHttpRequestHeader & req, quint64 size)
{
  req.setValue("Host", host);
  req.setValue("Connection", "Keep-Alive");
  if (size) {
    req.setContentLength(size);
    req.setContentType("text/xml; charset=utf-8");
  }
}

int
QWebdav::davRequest(QHttpRequestHeader & req, const QByteArray & data)
{
  setupHeaders(req, data.size());
  return request(req, data);
}

int
QWebdav::davRequest(QHttpRequestHeader & req, QIODevice * data)
{
  setupHeaders(req, data->size());
  return request(req, data);
}

int
QWebdav::mkdir ( const QString & dir )
{
  QHttpRequestHeader req("MKCOL", dir);

  return davRequest(req);
}

int
QWebdav::copy ( const QString & oldname, const QString & newname, bool overwrite)
{
  QHttpRequestHeader req("COPY", oldname);

  req.setValue("Destination", newname);
  req.setValue("Depth", "infinity");
  req.setValue("Overwrite", overwrite ? "T" : "F");
  return davRequest(req);
}

int
QWebdav::rename ( const QString & oldname, const QString & newname, bool overwrite)
{
  return move(oldname, newname);
}

int
QWebdav::move ( const QString & oldname, const QString & newname, bool overwrite)
{
  QHttpRequestHeader req("MOVE", oldname);

  req.setValue("Destination", newname);
  req.setValue("Depth", "infinity");
  req.setValue("Overwrite", overwrite ? "T" : "F");
  return davRequest(req);
}

int
QWebdav::rmdir ( const QString & dir )
{
  return remove(dir);
}

int
QWebdav::remove ( const QString & path )
{
  QHttpRequestHeader req("DELETE", path);

  return davRequest(req);
}

