#-------------------------------------------------
#
# Project created by QtCreator 2014-03-27T19:51:58
#
#-------------------------------------------------

QT       -= core gui

TARGET = ../../lib/fast-rtti
TEMPLATE = lib
CONFIG += staticlib

INCLUDEPATH += ../../include/


SOURCES += \
    ../../source/RTTI.cpp

HEADERS += \
    ../../include/RTTI.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
