#-------------------------------------------------
#
# Project created by prostoRoman 22.07.2021 when megre with linux-support.
#
#-------------------------------------------------

include(utility.pro)

QT       += core
QT       -= opengl gui
DEFINES  += QT_MESSAGELOGCONTEXT

CONFIG   += warn_on
CONFIG   += console

greaterThan(QT_MAJOR_VERSION, 4) {
  QT += serialport
}
lessThan(QT_MAJOR_VERSION, 5)  {
  CONFIG   += serialport # for Qt4
}
greaterThan(QT_MAJOR_VERSION, 5) {
  QT += core5compat # for Qt6
}

contains(ARCHITECTURE, x64) {
  TARGET = mdrfwucli64
}
contains(ARCHITECTURE, x86) {
  TARGET = mdrfwucli
}

TEMPLATE = app

SOURCES += mdrfwucli.cpp\
    hex.cpp \
    bytearray.cpp \
    mdruartloader.cpp \
    vasililoader.cpp

HEADERS  += \
    hex.h \
    bytearray.h \
    mdruartloader.h \
    mdrmcudescriptor.h \
    elf.h \
    vasililoader.h

# MOC (метаобъектный компилятор), вроде бы и умеет препроцессировать файлы, но поместить объявление класса в условную компиляцию нельзя.
lessThan(QT_MAJOR_VERSION, 5) {
    HEADERS  += signalhelper.h
}
equals(QT_MAJOR_VERSION, 5) {
  lessThan(QT_MINOR_VERSION, 2) {
    HEADERS  += signalhelper.h
  }
}

RESOURCES +=  mdrloader.qrc

DISTFILES += \
  mdrfwuart.bat

