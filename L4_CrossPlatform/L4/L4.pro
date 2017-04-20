QT += core
QT -= gui

TARGET = L4
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    project.cpp

DEFINES = _AFXDLL _MBCS

HEADERS += \
    project.h
