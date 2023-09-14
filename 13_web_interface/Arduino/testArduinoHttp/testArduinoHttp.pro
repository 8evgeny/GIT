# Корневой каталог исходников Arduino Core
ARDUINO_DIR=/home/evg/Arduino/libraries

TOOLS_PATH=/home/evg/toolchain/gcc-arm-none-eabi-new

LINKER_SCRIPT_FLASH=/home/evg/SOFT/Github/GIT/13_web_interface/Arduino/testArduinoHttp/STM32F407VGTX_FLASH.ld

# Выбираем целевой контроллер (Arduino Uno, Nano, Mini)

#ARDUINO_MCU=atmega328p
ARDUINO_MCU=stm32

# Частота тактирования контроллера
ARDUINO_FCPU = 16000000L

QT -= gui core

# Шаблон проекта - приложение, будет собираться исполняемый файл формата ELF
TEMPLATE = app

CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

DESTDIR = ../build
TARGET = hhtpServer

# Подключаем заголовочные файлы
INCLUDEPATH += $$ARDUINO_DIR/EthernetWebServer_SSL_STM32/src
INCLUDEPATH += $$ARDUINO_DIR/Ethernet_Generic/src
INCLUDEPATH += $$ARDUINO_DIR/Functional-Vlpp/src
INCLUDEPATH += $$ARDUINO_DIR/EthernetWebServer_SSL/Packages_Patches/realtek/hardware/AmebaD/3.0.10/cores/arduino/avr
INCLUDEPATH += $$ARDUINO_DIR/Ethernet_Generic/Packages_Patches/arduino/hardware/samd/1.8.8/cores
INCLUDEPATH +=$$TOOLS_PATH/arm-none-eabi/include
INCLUDEPATH += /usr/include/newlib/c++/9.2.1
#Компилятор C и его ключи
QMAKE_CC = $$TOOLS_PATH/bin/arm-none-eabi-gcc
QMAKE_CFLAGS += -mcpu=cortex-m4- std=gnu11 -DUSE_HAL_DRIVER -DSTM32F407xx -c -Os -ffunction-sections -fdata-sections
QMAKE_CFLAGS += -Wall -fstack-usage -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Xlinker -Map=$$TARGET.map

#Компилятор C++ и его ключи
QMAKE_CXX = $$TOOLS_PATH/bin/arm-none-eabi-g++
QMAKE_CXXFLAGS += -mcpu=cortex-m4 -DUSE_HAL_DRIVER -DSTM32F407xx -c -Os -ffunction-sections -fdata-sections
QMAKE_CXXFLAGS += -Wall -fstack-usage -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Xlinker -Map=$$TARGET.map

#Линкер и его ключи
QMAKE_LINK = $$TOOLS_PATH/bin/arm-none-eabi-g++
QMAKE_LFLAGS += -T$$LINKER_SCRIPT_FLASH -mcpu=cortex-m4 --specs=nosys.specs -Wl,--gc-sections -static --specs=nano.specs
QMAKE_LFLAGS += -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
QMAKE_LIBS = -lm

#Настраиваем постобработку ELF-файла
QMAKE_POST_LINK += $$TOOLS_PATH/bin/arm-none-eabi-objcopy -O ihex -j .text -j .data -S ${TARGET} ${TARGET}.hex

#HEADERS += $$files($$ARDUINO_DIR/cores/arduino/*.h)
#HEADERS += $$files($$ARDUINO_DIR/variants/standard/*.h)

SOURCES += \
    http.cpp
#SOURCES += $$files($$ARDUINO_DIR/cores/arduino/*.c)
#SOURCES += $$files($$ARDUINO_DIR/cores/arduino/*.cpp)
