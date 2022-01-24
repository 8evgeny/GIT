#-------------------------------------------------
#
# Project created by prostoRoman 2017-07-13T11:16:51
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
  TARGET = MdrFwUgui64
}
contains(ARCHITECTURE, x86) {
  TARGET = MdrFwUgui
}

TEMPLATE = app

SOURCES += mdrfwugui.cpp\

HEADERS  += \
    hex.h \
    mdrfwugui.h

FORMS    += \
    mdrfwuform.ui

unix: {
    RESOURCES += cli/linux/Qt$${QT_VERSION}/cli.qrc
}
win32: {
  contains(ARCHITECTURE, x64) {
    RESOURCES += cli/w64/Qt$${QT_VERSION}/cli64.qrc
  }
  contains(ARCHITECTURE, x86) {
    RESOURCES += cli/w32/Qt$${QT_VERSION}/cli.qrc
  }
}

message(Included resources is $${RESOURCES})
