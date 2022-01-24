QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES +=  IPS_USING_QT

INCLUDEPATH +=      \
    ..              \
    ../..         \
    ../../islp    \
    ../../names   \
    ../../tools   \
    ../multicaster/ineter

SOURCES += \
    ../multicaster/ineter/multicastsocket.cpp \
    main.cpp \
    process/msgprocessor.cpp \
    serverwindow.cpp \
    ../../islp/basicmessage.cpp \
    ../../islp/clientsm.cpp \
    ../../islp/setstationmessage.cpp \
    ../../islp/stationresponse.cpp \
    ../../islp/yellmsg.cpp \
    ../../names/ipsnames.cpp \
    ../../tools/arcstylecrypt.cpp \
    ../../tools/intercomstation.cpp \
    ../../tools/nswfl_crc32.cpp \
    ../multicaster/ineter/mcastRecieve.cpp \
    ../multicaster/ineter/mcastSend.cpp


HEADERS += \
    ../multicaster/ineter/multicastsocket.hpp \
    process/msgprocessor.hpp \
    serverwindow.hpp \
    ../../ips_helpers.hpp \
    ../../islp.hpp \
    ../../debug_macro.h \
    ../../islp/basicmessage.hpp \
    ../../islp/clientsm.hpp \
    ../../islp/details.hpp \
    ../../islp/doxydoc.hpp \
    ../../islp/setstationmessage.hpp \
    ../../islp/stationresponse.hpp \
    ../../islp/yellmsg.hpp \
    ../../names/ipsnames.hpp \
    ../../tools/arcstylecrypt.hpp \
    ../../tools/intercomstation.hpp \
    ../../tools/nswfl_crc32.hpp \
    ../multicaster/ineter/mcastRecieve.hpp \
    ../multicaster/ineter/mcastSend.hpp

FORMS += \
    serverwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
