#-------------------------------------------------
#
# Project created by QtCreator 2016-03-15T12:31:00
#
#-------------------------------------------------

QT += core gui serialport widgets multimedia charts

TARGET = AsptNonLinTest
TEMPLATE = app

CONFIG += c++11 #console

win32:RC_FILE = myapp.rc
win32:QT += winextras axcontainer

FORMS += mainwindow.ui

RESOURCES += res.qrc

win32:LIBS += -lwinmm

DISTFILES += \
    res/image10.png \
    res/icon1.svg \
    res/icon2.svg \
    res/icon3.svg \
    res/blank.xlsx \
    measuring_interface/ini.txt

HEADERS += \
    excel.h \
    mainwindow.h \
    measuring_interface/Aspt5/adccfg.h \
    measuring_interface/Aspt5/aspt.h \
    measuring_interface/Aspt5/global.h \
    measuring_interface/Upn/myprotocol.h \
    measuring_interface/Upn/upn.h \
    measuring_interface/common_interfaces.h \
    measuring_interface/measure.h \
    measuring_interface/mi.h \
    myheader.h \
    mytablemodel.h \
    rowdata.h \
    tableparams.h \
    mytable.h

SOURCES += \
    excel.cpp \
    main.cpp \
    mainwindow.cpp \
    measuring_interface/Aspt5/adccfg.cpp \
    measuring_interface/Aspt5/aspt.cpp \
    measuring_interface/Aspt5/global.cpp \
    measuring_interface/Upn/myprotocol.cpp \
    measuring_interface/Upn/upn.cpp \
    measuring_interface/measure.cpp \
    measuring_interface/mi.cpp \
    myheader.cpp \
    mytablemodel.cpp \
    rowdata.cpp \
    mytable.cpp



