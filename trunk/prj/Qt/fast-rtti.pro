#-------------------------------------------------
#
# Project created by QtCreator 2014-03-27T19:51:58
#
#-------------------------------------------------

QT       -= core gui

CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3
QMAKE_CXXFLAGS_RELEASE += -m64

TARGET = ../../lib/fast-rtti
TEMPLATE = lib
CONFIG += staticlib

INCLUDEPATH += /usr/local/include
INCLUDEPATH += ../../include/


SOURCES +=

HEADERS += \
    ../../include/RTTI.hpp
unix {
    target.path = /usr/lib
    INSTALLS += target
}
