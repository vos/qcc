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


SOURCES += main.cpp \
           mainwindow.cpp

HEADERS += mainwindow.h

FORMS += mainwindow.ui

RESOURCES += icons.qrc
