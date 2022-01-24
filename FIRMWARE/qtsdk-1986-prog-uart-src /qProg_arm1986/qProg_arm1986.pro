#-------------------------------------------------
#
# Project created by QtCreator 2014-05-01T19:38:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qProg_arm1986
TEMPLATE = app


SOURCES += main.cpp\
        qprogarmdialog.cpp \
    unit_lib.cpp \
    work_object.cpp

HEADERS  += qprogarmdialog.h \
    glob_var.h \
    unit_lib.h \
    work_object.h

FORMS    += qprogarmdialog.ui

RESOURCES += \
    qProg_arm1986.qrc
