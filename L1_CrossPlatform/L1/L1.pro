QT += core
QT -= gui

TARGET = L1
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    human.cpp \
    processunit.cpp \
    cashmachine.cpp
DEFINES = _AFXDLL _MBCS

HEADERS += \
    human.h \
    processunit.h \
    cashmachine.h
