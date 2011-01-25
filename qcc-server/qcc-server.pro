#-------------------------------------------------
#
# Project created by QtCreator 2011-01-23T16:09:56
#
#-------------------------------------------------

QT = core network xml

TARGET = qcc-server
TEMPLATE = app
CONFIG += console link_prl
CONFIG -= app_bundle

CONFIG(debug, debug|release) {
    DEFINES += DEBUG
    LIBQCCPATH = ../qcc-core-build-desktop/debug
} else {
    DEFINES += RELEASE
    LIBQCCPATH = ../qcc-core-build-desktop/release
}

INCLUDEPATH += ../qcc-core
LIBS += -L$$LIBQCCPATH -lqcc-core


HEADERS += server.h \
           user.h

SOURCES += main.cpp \
           server.cpp \
           user.cpp
