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
    Settings.cpp \
    LogHandler.cpp \
    MagicAssistant.cpp \
    NBAAssistant.cpp \
    ToolBar.cpp \
    ToolButton.cpp \
    InfoBoard.cpp

HEADERS  += \
    defines.h \
    Settings.h \
    LogHandler.h \
    Singleton.h \
    MagicAssistant.h \
    NBAAssistant.h \
    ToolBar.h \
    ToolButton.h \
    InfoBoard.h

FORMS    += \
    ToolBar.ui \
    MagicAssistant.ui \
    InfoBoard.ui

RESOURCES += \
    resource.qrc

macx: ICON = images/app.icns
win32: RC_ICONS = images/james_16.ico
