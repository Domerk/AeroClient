#-------------------------------------------------
#
# Project created by QtCreator 2015-11-23T21:12:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AeroClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../mongo/mongo-client-install/lib/release/ -lmongoclient
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../mongo/mongo-client-install/lib/debug/ -lmongoclient
else:unix: LIBS += -L$$PWD/../../mongo/mongo-client-install/lib/ -lmongoclient

INCLUDEPATH += $$PWD/../../mongo/mongo-client-install/include
DEPENDPATH += $$PWD/../../mongo/mongo-client-install/include
INCLUDEPATH += -I /usr/include/boost/
LIBS += -L /usr/lib/ -l:libboost_system.so -l:libboost_regex.so -l:libboost_thread.so

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../mongo/mongo-client-install/lib/release/libmongoclient.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../mongo/mongo-client-install/lib/debug/libmongoclient.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../mongo/mongo-client-install/lib/release/mongoclient.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../mongo/mongo-client-install/lib/debug/mongoclient.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../mongo/mongo-client-install/lib/libmongoclient.a
