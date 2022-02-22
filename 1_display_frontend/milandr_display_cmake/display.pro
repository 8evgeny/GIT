# Ни гуи, ни ядра Qt нам не надо!
QT -= gui core
CONFIG -= qt

# Шаблон проекта - приложение, будет собираться исполняемый файл формата ELF
TEMPLATE = app

#Задаем каталог для собранного бинарника и его имя
DESTDIR = ../bin
TARGET = display

#Дальше подключим директории поиска заголовочных файлов
# Подключаем заголовочные файлы

INCLUDEPATH += ../MDR1986BExx/2.0.3/Libraries/CMSIS/MDR32Fx/DeviceSupport/MDR1986VE9x/inc
INCLUDEPATH += ../MDR1986BExx/2.0.3/Libraries/CMSIS/MDR32Fx/DeviceSupport/MDR1986VE9x/startup/arm
INCLUDEPATH += ../MDR1986BExx/2.0.3/Libraries/CMSIS/MDR32Fx/CoreSupport/CM3
INCLUDEPATH += ../MDR1986BExx/2.0.3/Libraries/SPL/MDR32Fx
INCLUDEPATH += ../MDR1986BExx/2.0.3/Libraries/SPL/MDR32Fx/inc
INCLUDEPATH += ../MDR1986BExx/2.0.3/Libraries/SPL/MDR32Fx/inc/USB_Library
INCLUDEPATH += ../MDR1986BExx/2.0.3/Libraries/MDR32Fx/inc
INCLUDEPATH += ../MDR1986BExx/2.0.3/Libraries/SPL/MDR32Fx/src
INCLUDEPATH += ./include
INCLUDEPATH += .

#Компилятор C и его ключи
QMAKE_CC = arm-none-eabi-gcc
QMAKE_CFLAGS += -Wall
QMAKE_CFLAGS += -mcpu=cortex-m3
QMAKE_CFLAGS += -mthumb
QMAKE_CFLAGS += -D__HEAP_SIZE=0x0C00
QMAKE_CFLAGS += -D__STACK_SIZE=0x0400
QMAKE_CFLAGS += -mfloat-abi=soft
QMAKE_CFLAGS += -fno-strict-aliasing
QMAKE_CFLAGS += -fdata-sections
QMAKE_CFLAGS += -ffunction-sections
#QMAKE_CFLAGS += -xc
#QMAKE_CFLAGS += -std=c99
#QMAKE_CFLAGS += --target=arm-arm-none-eabi
#QMAKE_CFLAGS += -c
#QMAKE_CFLAGS += -fno-rtti
#QMAKE_CFLAGS += -funsigned-char
#QMAKE_CFLAGS += -fshort-enums
#QMAKE_CFLAGS += -fshort-wchar

#Компилятор C++ и его ключи
QMAKE_CXX = arm-none-eabi-g++
#QMAKE_CXXFLAGS +=
#QMAKE_CXXFLAGS +=
#QMAKE_CXXFLAGS +=

#задаем компоновщик и его ключи
QMAKE_LINK = arm-none-eabi-gcc
QMAKE_LFLAGS += --specs=nosys.specs
QMAKE_LFLAGS += --specs=nano.specs
QMAKE_LFLAGS += -mcpu=cortex-m3
QMAKE_LFLAGS += -mthumb
QMAKE_LFLAGS += -Wl,--defsym=__HEAP_SIZE=0x0000
QMAKE_LFLAGS += -Wl,--defsym=__STACK_SIZE=0x0100
QMAKE_LFLAGS += -mfloat-abi=soft
QMAKE_LFLAGS += -fno-strict-aliasing
QMAKE_LFLAGS += -fdata-sections
QMAKE_LFLAGS += -ffunction-sections
QMAKE_LFLAGS += -Wl,--gc-sections
#QMAKE_LFLAGS += -Wl,-script="./hardware/stm32f103c8_flash.ld"
#QMAKE_LFLAGS += -Wl,-Map=$(TARGET).map
#QMAKE_LFLAGS += -u _printf_float

QMAKE_LIBS = -lm

#Настраиваем постобработку ELF-файла, с целью перекрутить его в Intel HEX для последующей прошивки в плату
QMAKE_POST_LINK += arm-none-eabi-objcopy -O ihex -j .text -j .data -S ${TARGET} ${TARGET}.hex

# Заголовки
HEADERS += $$files(Device/include/*.h)
HEADERS += $$files(Drivers/include/*.h)
#HEADERS += Drivers/include/MDR32Fx.h

# Исходники
SOURCES += $$files($$DEVICE/*.c)
SOURCES += $$files($$DRIVERS/*.c)

#Заголовки проекта
INCLUDEPATH += ./include
HEADERS += $$files(./include/*.h)

# Исходники проекта
SOURCES += $$files(./src/*.c)
