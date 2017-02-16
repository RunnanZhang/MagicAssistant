#-------------------------------------------------
#
# Project created by QtCreator 2015-11-11T12:28:21
#
#-------------------------------------------------

QT       += core gui network quick xml widgets

TARGET = MagicAssistant
TEMPLATE = app

CONFIG += c++11

LIBS += User32.lib

SOURCES += main.cpp\
        magicassistant.cpp \
    toolbar.cpp \
    toolbutton.cpp \
    nbaassistant.cpp \
    Settings.cpp \
    LogHandler.cpp

HEADERS  += magicassistant.h \
    toolbar.h \
    toolbutton.h \
    defines.h \
    nbaassistant.h \
    Settings.h \
    LogHandler.h \
    Singleton.h

FORMS    += magicassistant.ui \
    toolbar.ui

RESOURCES += \
    resource.qrc

macx: ICON = images/app.icns
win32: RC_ICONS = images/james_16.ico
