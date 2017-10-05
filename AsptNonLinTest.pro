#-------------------------------------------------
#
# Project created by QtCreator 2016-03-15T12:31:00
#
#-------------------------------------------------

QT       += core gui serialport widgets printsupport multimedia

TARGET = AsptNonLinTest
TEMPLATE = app

#QMAKE_LFLAGS_DEBUG += /INCREMENTAL:NO
#DEFINES += EXCEL

CONFIG += c++11 #console

win32:RC_FILE = myapp.rc
win32:QT += winextras axcontainer

SOURCES += \
    aspt.cpp \
    aspt_upn.cpp \
    excel.cpp \
    main.cpp\
    mainwindow.cpp \
    myprotocol.cpp \
    qcustomplot.cpp \
    table.cpp

HEADERS  += \
    aspt.h \
    aspt_upn.h \
    excel.h \
    mainwindow.h \
    myprotocol.h \
    qcustomplot.h \
    table.h


FORMS    += mainwindow.ui

#RESOURCES += res.qrc

win32:LIBS += -lwinmm

DISTFILES += \
    res/image10.png \
    res/icon1.svg \
    res/icon2.svg \
    res/icon3.svg \
    res/blank.xlsx
