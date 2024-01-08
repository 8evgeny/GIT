cmake_minimum_required(VERSION 3.5)

set(CMAKE_OBJCOPY /usr/bin/avr-objcopy)

set(nameFirmware test_mega64_gpio)
project(${nameFirmware} LANGUAGES C)
add_definitions(-D__AVR_ATmega64__)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}\
    -mmcu=atmega64 -DDEBUG -O1 -fdata-sections -ffunction-sections -fpack-struct -fshort-enums \
    -Wmissing-prototypes -Werror-implicit-function-declaration -Wpointer-arith  \
    -g3 -Wall -std=gnu99 -mrelax  -fno-strict-aliasing -Wstrict-prototypes \
    ")
#-x c -DDEBUG -DBOARD=STK600_MEGA  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.2.150\include" -I"../src/ASF/common/boards" -I"../src/ASF/mega/utils/preprocessor" -I"../src/ASF/mega/utils" -I"../src/ASF/common/utils" -I"../src" -I"../src/config"  -O1 -fdata-sections -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax -g3 -Wall -mmcu=atmega128 -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.2.150\gcc\dev\atmega128" -c -std=gnu99 -fno-strict-aliasing -Wstrict-prototypes -Wmissing-prototypes -Werror-implicit-function-declaration -Wpointer-arith -mrelax -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"

add_executable(${nameFirmware} main.c)

set(HEX_FILE ${CMAKE_BINARY_DIR}/${nameFirmware}.hex)
set(BIN_FILE ${CMAKE_BINARY_DIR}/${nameFirmware}.bin)

add_custom_command(TARGET ${nameFirmware} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${nameFirmware}> ${HEX_FILE}
    COMMENT "Building ${HEX_FILE} ")

add_custom_command(TARGET ${nameFirmware} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${nameFirmware}> ${BIN_FILE}
    COMMENT "Building ${BIN_FILE} ")

#add_custom_command(TARGET ${nameFirmware} POST_BUILD
#    COMMAND avrdude -p m64 -c usbasp -B 4 -P usb -U flash:w:"${CMAKE_BINARY_DIR}/${nameFirmware}.hex":a
#    COMMENT "Write Flash ")

add_custom_command(TARGET ${nameFirmware} POST_BUILD
    COMMAND avrdude -p m64 -c jtag1 -B 4 -P /dev/ttyUSB0 -U flash:w:"${CMAKE_BINARY_DIR}/${nameFirmware}.hex":a
    COMMENT "Write Flash ")
