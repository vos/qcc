#-------------------------------------------------
#
# Project created by QtCreator 2011-01-25T13:46:15
#
#-------------------------------------------------

QT = core network

TARGET = qcc-core
TEMPLATE = lib
CONFIG += staticlib create_prl


HEADERS += qccpacket.h

SOURCES += qccpacket.cpp


unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/local/lib
    }
    INSTALLS += target
}
