QT += quick multimedia sql testlib network core gui widgets multimedia serialport
CONFIG += c++14

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


 # ~/Qt/5.12.5/gcc_64/bin/lupdate assignment_list.qml firmware.qml groups.qml network.qml main.qml main_menu.qml subscriber.qml subscriber_navigation.qml subscriber_window.qml update_rs232.qml assignment_list_firmware.qml assignment_list_rs232.qml verification.qml programmer_window.qml status_window.qml change_window.qml -ts translator/main_ru.ts

TRANSLATIONS += translator/main_ru.ts
unix {
    SOURCES += \
            Lib/qjson/src/cmdline_tester.cpp \
            Lib/qjson/src/cmdlineparser.cpp \
            Lib/qjson/src/json_parser.cc \
            Lib/qjson/src/json_scanner.cc \
            Lib/qjson/src/json_scanner.cpp \
            Lib/qjson/src/parser.cpp \
            Lib/qjson/src/parserrunnable.cpp \
            Lib/qjson/src/qobjecthelper.cpp \
            Lib/qjson/src/serializer.cpp \
            Lib/qjson/src/serializerrunnable.cpp \
            Lib/aes/qaesencryption.cpp \
            appcore.cpp \
            assignment_list.cpp \
            firmware.cpp \
            groups.cpp \
            main.cpp \
            network.cpp \
            programmer.cpp \
            receiver.cpp \
            subscriber.cpp \
            terminal.cpp \
            verification.cpp
}

win32:SOURCES += "C:\Users\Professional\Documents\GIT1\6_promwad2_step_by_step\Software_configurator\configurator\Lib\qjson\src\parserrunnable.cpp"
win32:SOURCES += "C:\Users\Professional\Documents\GIT1\6_promwad2_step_by_step\Software_configurator\configurator\Lib\qjson\src\cmdline_tester.cpp"
win32:SOURCES += "C:\Users\Professional\Documents\GIT1\6_promwad2_step_by_step\Software_configurator\configurator\Lib\qjson\src\cmdlineparser.cpp"
win32:SOURCES += "C:\Users\Professional\Documents\GIT1\6_promwad2_step_by_step\Software_configurator\configurator\Lib\qjson\src\json_parser.cc"
win32:SOURCES += "C:\Users\Professional\Documents\GIT1\6_promwad2_step_by_step\Software_configurator\configurator\Lib\qjson\src\json_scanner.cc"
win32:SOURCES += "C:\Users\Professional\Documents\GIT1\6_promwad2_step_by_step\Software_configurator\configurator\Lib\qjson\src\json_scanner.cpp"
win32:SOURCES += "C:\Users\Professional\Documents\GIT1\6_promwad2_step_by_step\Software_configurator\configurator\Lib\qjson\src\parser.cpp"
win32:SOURCES += "C:\Users\Professional\Documents\GIT1\6_promwad2_step_by_step\Software_configurator\configurator\Lib\qjson\src\qobjecthelper.cpp"
win32:SOURCES += "C:\Users\Professional\Documents\GIT1\6_promwad2_step_by_step\Software_configurator\configurator\Lib\qjson\src\serializer.cpp"
win32:SOURCES += "C:\Users\Professional\Documents\GIT1\6_promwad2_step_by_step\Software_configurator\configurator\Lib\qjson\src\serializerrunnable.cpp"
win32:SOURCES += "C:\Users\Professional\Documents\GIT1\6_promwad2_step_by_step\Software_configurator\configurator\Lib\aes\qaesencryption.cpp"

win32:SOURCES += \
            appcore.cpp \
            assignment_list.cpp \
            firmware.cpp \
            groups.cpp \
            main.cpp \
            network.cpp \
            programmer.cpp \
            receiver.cpp \
            subscriber.cpp \
            terminal.cpp \
            verification.cpp

unix:RESOURCES += qml.qrc \
            icons/GIT_en.png \
            icons/git-logo.png \
            qtquickcontrols2.conf

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:DISTFILES += \
    icons/GIT_en.png \
    icons/GIT_en.svg \
    icons/git-logo.png \
    qtquickcontrols2.conf


unix:INCLUDEPATH += \
    Lib/qjson/include/ \
    Lib/aes/

unix:LIBS +=  -lboost_filesystem -lboost_system

win32:INCLUDEPATH += "C:\Users\Professional\Documents\GIT1\6_promwad2_step_by_step\Software_configurator\configurator\Lib\aes"

win32:INCLUDEPATH += C:/boost/boost_1_66_0

win32 {
    INCLUDEPATH += C:/boost/boost_1_66_0
    LIBS += "-LC:/boost/boost_1_66_0/stage/lib/" \
    "-Llibboost_system-mgw112-mt-x64-1_66.a", "-Llibboost_filesystem-mgw112-mt-d-x64-1_66.a" -LLIBS
}


unix:HEADERS += \
    Lib/qjson/include/QJson/Parser \
    Lib/qjson/include/QJson/QObjectHelper \
    Lib/qjson/include/QJson/Serializer \
    Lib/qjson/src/FlexLexer.h \
    Lib/qjson/src/cmdlineparser.h \
    Lib/qjson/src/json_parser.hh \
    Lib/qjson/src/json_scanner.h \
    Lib/qjson/src/location.hh \
    Lib/qjson/src/parser.h \
    Lib/qjson/src/parser_p.h \
    Lib/qjson/src/parserrunnable.h \
    Lib/qjson/src/position.hh \
    Lib/qjson/src/qjson_debug.h \
    Lib/qjson/src/qjson_export.h \
    Lib/qjson/src/qobjecthelper.h \
    Lib/qjson/src/serializer.h \
    Lib/qjson/src/serializerrunnable.h \
    Lib/qjson/src/stack.hh \
    Lib/aes/qaesencryption.h \
    RuLang.h \
    RuTranslator.h \
    appcore.h \
    receiver.h

win32:HEADERS += "C:\Users\Professional\Documents\GIT1\6_promwad2_step_by_step\Software_configurator\configurator\Lib\qjson\include\QJson\Parser"
win32:HEADERS += "C:\Users\Professional\Documents\GIT1\6_promwad2_step_by_step\Software_configurator\configurator\Lib\qjson\include\QJson\QObjectHelper"
win32:HEADERS += "C:\Users\Professional\Documents\GIT1\6_promwad2_step_by_step\Software_configurator\configurator\Lib\qjson\include\QJson\Serializer"

win32:HEADERS += "C:\Users\Professional\Documents\GIT1\6_promwad2_step_by_step\Software_configurator\configurator\Lib\qjson\src\FlexLexer.h"
win32:HEADERS += "C:\Users\Professional\Documents\GIT1\6_promwad2_step_by_step\Software_configurator\configurator\Lib\qjson\src\cmdlineparser.h"
win32:HEADERS += "C:\Users\Professional\Documents\GIT1\6_promwad2_step_by_step\Software_configurator\configurator\Lib\qjson\src\json_parser.hh"
win32:HEADERS += "C:\Users\Professional\Documents\GIT1\6_promwad2_step_by_step\Software_configurator\configurator\Lib\qjson\src\json_scanner.h"
win32:HEADERS += "C:\Users\Professional\Documents\GIT1\6_promwad2_step_by_step\Software_configurator\configurator\Lib\qjson\src\location.hh"
win32:HEADERS += "C:\Users\Professional\Documents\GIT1\6_promwad2_step_by_step\Software_configurator\configurator\Lib\qjson\src\parser.h"
win32:HEADERS += "C:\Users\Professional\Documents\GIT1\6_promwad2_step_by_step\Software_configurator\configurator\Lib\qjson\src\parser_p.h"
win32:HEADERS += "C:\Users\Professional\Documents\GIT1\6_promwad2_step_by_step\Software_configurator\configurator\Lib\qjson\src\parserrunnable.h"
win32:HEADERS += "C:\Users\Professional\Documents\GIT1\6_promwad2_step_by_step\Software_configurator\configurator\Lib\qjson\src\position.hh"
win32:HEADERS += "C:\Users\Professional\Documents\GIT1\6_promwad2_step_by_step\Software_configurator\configurator\Lib\qjson\src\qjson_debug.h"
win32:HEADERS += "C:\Users\Professional\Documents\GIT1\6_promwad2_step_by_step\Software_configurator\configurator\Lib\qjson\src\qjson_export.h"
win32:HEADERS += "C:\Users\Professional\Documents\GIT1\6_promwad2_step_by_step\Software_configurator\configurator\Lib\qjson\src\qobjecthelper.h"
win32:HEADERS += "C:\Users\Professional\Documents\GIT1\6_promwad2_step_by_step\Software_configurator\configurator\Lib\qjson\src\serializer.h"
win32:HEADERS += "C:\Users\Professional\Documents\GIT1\6_promwad2_step_by_step\Software_configurator\configurator\Lib\qjson\src\serializerrunnable.h"
win32:HEADERS += "C:\Users\Professional\Documents\GIT1\6_promwad2_step_by_step\Software_configurator\configurator\Lib\qjson\src\stack.hh"
win32:HEADERS += "C:\Users\Professional\Documents\GIT1\6_promwad2_step_by_step\Software_configurator\configurator\Lib\aes\qaesencryption.h"

win32:HEADERS += RuLang.h \
                RuTranslator.h \
                appcore.h \
                receiver.h
