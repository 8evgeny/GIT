TEMPLATE = app

CONFIG += qt console release thread
CONFIG -= app_bundle

QT += core
QT -= gui

TARGET = mcufmloader

INCLUDEPATH = .

HEADERS += \
    src/bastypes.h \
    src/common.h \
    src/FmConverter.h \
    src/MCUFmLoader.h \
    src/mcus.h \
    src/RS232.h

SOURCES += 	\
		src/main.cpp \
    src/FmConverter.cpp \
    src/MCUFmLoader.cpp \
    src/RS232.cpp \
    src/special.cpp \
    src/common.c \
    src/mcus.c
