cmake_minimum_required(VERSION 3.5)

set(CMAKE_OBJCOPY /usr/bin/avr-objcopy)

set(nameFirmware test_asm_1)
project(${nameFirmware} LANGUAGES C ASM)

#SET(ASM_OPTIONS "-x assembler-with-cpp")
#SET(CMAKE_ASM_FLAGS "${CFLAGS} ${ASM_OPTIONS}" )

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}\
    -mmcu=atmega128 -DDEBUG -O1 -fdata-sections -ffunction-sections -fpack-struct -fshort-enums \
    -Wmissing-prototypes -Werror-implicit-function-declaration -Wpointer-arith  \
    -g3 -Wall -std=gnu99 -mrelax  -fno-strict-aliasing -Wstrict-prototypes \
    ")
include_directories(/usr/share/avra)
file(GLOB  src  *.c)
add_executable(fake_c ${src})


add_custom_command(TARGET fake_c POST_BUILD
    COMMAND cp ${CMAKE_SOURCE_DIR}/${nameFirmware}.asm ${CMAKE_BINARY_DIR}/${nameFirmware}.asm
    COMMENT "Building ${HEX_FILE} ")

add_custom_command(TARGET fake_c POST_BUILD
#    COMMAND avra ${CMAKE_BINARY_DIR}/${nameFirmware}.asm
    COMMAND ${CMAKE_SOURCE_DIR}/../gavrasm_lin_x86_64/gavrasm ${CMAKE_BINARY_DIR}/${nameFirmware}.asm
    COMMENT "Building ${HEX_FILE} ")

#add_custom_command(TARGET fake_c POST_BUILD
#    COMMAND avrdude -p m128 -c usbasp -B 4 -P usb -U flash:w:"${CMAKE_BINARY_DIR}/${nameFirmware}.hex":a
#    COMMENT "Write Flash ")




