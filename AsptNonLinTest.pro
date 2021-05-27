#-------------------------------------------------
# Project created by QtCreator 2016-03-15T12:31:00
#-------------------------------------------------
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QT += core gui serialport widgets multimedia charts

TARGET = AsptNonLinTest_V2
TEMPLATE = app

#CONFIG += c++17 #console
QMAKE_CXXFLAGS += /std:c++latest

win32:LIBS += -lwinmm
win32:QT += winextras axcontainer
win32:RC_FILE = myapp.rc

DESTDIR = $$_PRO_FILE_PWD_/bin

FORMS += mainwindow.ui

include(../aspt/aspt.pri)
#include(excel/excel.pri)
include(CommonInterfaces/CommonInterfaces.pri)
include(MyProtokol/XrProtokol.pri)

RESOURCES += \
    res/res.qrc

DISTFILES += \
    res/blank.xlsx \
    measuring_interface/ini.txt

HEADERS += \
    mainwindow.h \
    measuremodel.h \
    measuring_interface/Upn/upn.h \
    measuring_interface/devfinder.h \
    measuring_interface/measure.h \
    measuring_interface/mi.h \
    myheader.h \
    mytable.h \
    recent.h \
    rowdata.h \
    tableparams.h \

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    measuremodel.cpp \
    measuring_interface/Upn/upn.cpp \
    measuring_interface/devfinder.cpp \
    measuring_interface/measure.cpp \
    measuring_interface/mi.cpp \
    myheader.cpp \
    mytable.cpp \
    recent.cpp \
    rowdata.cpp \

