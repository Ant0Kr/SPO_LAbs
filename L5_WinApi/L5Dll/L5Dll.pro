#-------------------------------------------------
#
# Project created by QtCreator 2017-04-21T12:05:47
#
#-------------------------------------------------

QT       -= core gui

TARGET = L5Dll
TEMPLATE = lib

DEFINES += L5DLL_EXPORTS

SOURCES += l5dll.cpp

HEADERS += l5dll.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DEFINES = _AFXDLL _MBCS
