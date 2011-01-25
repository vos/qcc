#-------------------------------------------------
#
# Project created by QtCreator 2011-01-25T13:46:15
#
#-------------------------------------------------

QT = core

TARGET = qcc-core
TEMPLATE = lib
CONFIG += staticlib create_prl

SOURCES +=

HEADERS += \
    qccnamespace.h
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/local/lib
    }
    INSTALLS += target
}
