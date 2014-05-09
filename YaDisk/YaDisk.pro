#-------------------------------------------------
#
# Project created by QtCreator 2014-02-21T21:41:24
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = YaDisk
TEMPLATE = app


SOURCES += main.cpp\
    ClientRequests.cpp \
    CheckAndSync.cpp \
    TrayIcon.cpp

HEADERS  += ClientRequests.h \
    CheckAndSync.h \
    TrayIcon.h

FORMS    +=

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/qwebdav/release/ -lqwebdav
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/qwebdav/debug/ -lqwebdav
#else:unix: LIBS += -L$$PWD/qwebdav/ -lqwebdav

#INCLUDEPATH += $$PWD/qwebdav
#DEPENDPATH += $$PWD/qwebdav
