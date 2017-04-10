QT += core
QT -= gui

TARGET = User
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    user.cpp \
    processunit.cpp
DEFINES = _AFXDLL _MBCS
HEADERS += \
    user.h \
    processunit.h

