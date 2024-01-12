cmake_minimum_required(VERSION 3.5)

set(CMAKE_OBJCOPY /usr/bin/avr-objcopy)

set(nameFirmware DX_blink)
project(${nameFirmware} LANGUAGES C)
add_definitions(-D__AVR_ATmega128__)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}\
    -mmcu=atmega128 -DDEBUG -O1 -fdata-sections -ffunction-sections -fpack-struct -fshort-enums \
    -Wmissing-prototypes -Werror-implicit-function-declaration -Wpointer-arith  \
    -g3 -Wall -std=gnu99 -mrelax  -fno-strict-aliasing -Wstrict-prototypes \
    ")

add_executable(${nameFirmware} main.c)

set(HEX_FILE ${CMAKE_BINARY_DIR}/${nameFirmware}.hex)
set(BIN_FILE ${CMAKE_BINARY_DIR}/${nameFirmware}.bin)

add_custom_command(TARGET ${nameFirmware} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${nameFirmware}> ${HEX_FILE}
    COMMENT "Building ${HEX_FILE} ")

add_custom_command(TARGET ${nameFirmware} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${nameFirmware}> ${BIN_FILE}
    COMMENT "Building ${BIN_FILE} ")

add_custom_command(TARGET ${nameFirmware} POST_BUILD
    COMMAND avrdude -p m128 -B 20 -c jtag2isp -P usb -U flash:w:"/home/evg/SOFT/Github/GIT/21_MT9092_to_STM_GITLAB/VX-Mega128/Projects/DX_blink/build/${nameFirmware}.hex":a
    COMMENT "Write Flash ")



