#Светодиодная мигалка  ассемблер  gnu -  avr-as  Сборка CMAKE
#https://count-zero.ru/2018/gnu_assembler/
cmake_minimum_required(VERSION 3.5)

set(CMAKE_OBJCOPY /usr/bin/avr-objcopy)
set(CMAKE_OBJDUMP /usr/bin/avr-objdump)
set(nameFirmware test_asm_IDA)
project(${nameFirmware} LANGUAGES C ASM)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}\
    -mmcu=atmega128 -D__AVR_ATmega128__ -DDEBUG -O1 -fdata-sections -ffunction-sections -fpack-struct -fshort-enums \
    -Wmissing-prototypes -Werror-implicit-function-declaration -Wpointer-arith  \
    -g3 -Wall -std=gnu99 -mrelax  -fno-strict-aliasing -Wstrict-prototypes \
    ")
include_directories(/usr/lib/avr/include)
file(GLOB  src  *.s)
set_property(SOURCE ${src} APPEND PROPERTY COMPILE_OPTIONS "-x" "assembler-with-cpp")
add_executable(${PROJECT_NAME} ${src})

set(HEX_FILE ${CMAKE_BINARY_DIR}/${nameFirmware}.hex)
set(DIS ${CMAKE_BINARY_DIR}/${nameFirmware}.dis)
add_custom_command(TARGET ${nameFirmware} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${nameFirmware}> ${HEX_FILE}
    COMMENT "Building ${HEX_FILE} ")
add_custom_command(TARGET ${nameFirmware} POST_BUILD
    COMMAND avrdude -p m128 -c usbasp -B 4 -P usb -U flash:w:"${CMAKE_BINARY_DIR}/${nameFirmware}.hex":a
    COMMENT "Write Flash ")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJDUMP} -m avr -D ${CMAKE_BINARY_DIR}/${nameFirmware}.hex > ${DIS}
    COMMENT "Disassemling ")
