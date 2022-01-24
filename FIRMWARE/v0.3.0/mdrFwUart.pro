#-------------------------------------------------
#
# Project created by QtCreator 2017-07-13T11:16:51
#
#-------------------------------------------------

include(utility.pro)

QT       += core gui
QT       -= opengl
DEFINES  += QT_MESSAGELOGCONTEXT

CONFIG   += warn_on

greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets serialport
}
lessThan(QT_MAJOR_VERSION, 5)  {
  CONFIG   += serialport # for Qt4
}
greaterThan(QT_MAJOR_VERSION, 5) {
  QT += core5compat # for Qt6
}

contains(ARCHITECTURE, x64) {
  TARGET = MdrFwUart64
}
contains(ARCHITECTURE, x86) {
  TARGET = MdrFwUart
}

TEMPLATE = app

SOURCES += \
    hex.cpp \
    bytearray.cpp \
    mdruartloader.cpp \
    vasililoader.cpp \
    mdrfwuart.cpp

HEADERS  += \
    hex.h \
    bytearray.h \
    mdruartloader.h \
    mdrmcudescriptor.h \
    elf.h \
    vasililoader.h \
    mdrfwuart.h

FORMS    += \
    mdrfwuform.ui

RESOURCES +=  mdrloader.qrc

DISTFILES += \
  mdrfwuart.bat

