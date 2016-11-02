#-------------------------------------------------
#
# Project created by QtCreator 2015-11-11T12:28:21
#
#-------------------------------------------------

QT       += core gui network quick

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MagicAssistant
TEMPLATE = app

CONFIG += c++11

LIBS += User32.lib

SOURCES += main.cpp\
        magicassistant.cpp \
    toolbar.cpp \
    toolbutton.cpp \
    nbaassistant.cpp

HEADERS  += magicassistant.h \
    toolbar.h \
    toolbutton.h \
    defines.h \
    nbaassistant.h

FORMS    += magicassistant.ui \
    toolbar.ui

RESOURCES += \
    resource.qrc

macx: ICON = images/app.icns
win32: RC_ICONS = images/james_16.ico
