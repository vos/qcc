#-------------------------------------------------
#
# Project created by QtCreator 2011-01-23T16:10:20
#
#-------------------------------------------------

QT = core gui network

TARGET = qcc
TEMPLATE = app
CONFIG += link_prl

CONFIG(debug, debug|release) {
    DEFINES += DEBUG
    LIBQCCPATH = ../qcc-core-build-desktop/debug
} else {
    DEFINES += RELEASE
    LIBQCCPATH = ../qcc-core-build-desktop/release
}

INCLUDEPATH += ../qcc-core
LIBS += -L$$LIBQCCPATH -lqcc-core


HEADERS += mainwindow.h \
           messagewindow.h \
           messagepage.h \
           contact.h \
           contactlistmodel.h

SOURCES += main.cpp \
           mainwindow.cpp \
           messagewindow.cpp \
           messagepage.cpp \
           contact.cpp \
           contactlistmodel.cpp

FORMS += mainwindow.ui \
         messagewindow.ui \
         messagepage.ui

RESOURCES += icons.qrc
