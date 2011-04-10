#-------------------------------------------------
#
# Project created by QtCreator 2011-01-23T16:10:20
#
#-------------------------------------------------

QT = core gui network

TARGET = qcc
TEMPLATE = app
CONFIG += link_prl crypto

CONFIG(debug, debug|release) {
    DEFINES += DEBUG
    QCC_PREFIX = ../qcc-core-build-desktop/debug
} else {
    DEFINES += RELEASE
    QCC_PREFIX = ../qcc-core-build-desktop/release
}

INCLUDEPATH += ../qcc-core
LIBS += -L$$QCC_PREFIX -lqcc-core

HEADERS += mainwindow.h \
           registerdialog.h \
           messagewindow.h \
           messagepage.h \
           contact.h \
           contactlistmodel.h

SOURCES += main.cpp \
           mainwindow.cpp \
           registerdialog.cpp \
           messagewindow.cpp \
           messagepage.cpp \
           contact.cpp \
           contactlistmodel.cpp

FORMS += mainwindow.ui \
         registerdialog.ui \
         messagewindow.ui \
         messagepage.ui

RESOURCES += icons.qrc

OTHER_FILES += ../README.md
