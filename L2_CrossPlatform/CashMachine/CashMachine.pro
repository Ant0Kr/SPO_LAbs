QT += core
QT -= gui

TARGET = CashMachine
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    cashmachine.cpp \
    processunit.cpp
DEFINES = _AFXDLL _MBCS
HEADERS += \
    cashmachine.h \
    processunit.h

