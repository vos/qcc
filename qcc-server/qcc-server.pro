#-------------------------------------------------
#
# Project created by QtCreator 2011-01-23T16:09:56
#
#-------------------------------------------------

QT += core network xml
QT -= gui

TARGET = qcc-server
CONFIG += console
CONFIG -= app_bundle
CONFIG(debug, debug|release):DEFINES += DEBUG

TEMPLATE = app


HEADERS += server.h \
           user.h

SOURCES += main.cpp \
           server.cpp \
           user.cpp
