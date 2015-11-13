#-------------------------------------------------
#
# Project created by QtCreator 2015-11-11T12:28:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MagicAssistant
TEMPLATE = app


SOURCES += main.cpp\
        magicassistant.cpp \
    toolbar.cpp \
    toolbutton.cpp

HEADERS  += magicassistant.h \
    toolbar.h \
    toolbutton.h \
    defines.h

FORMS    += magicassistant.ui \
    toolbar.ui

RESOURCES += \
    resource.qrc

macx: ICON = images/app.icns
win32: RC_ICONS = images/james_16.ico
