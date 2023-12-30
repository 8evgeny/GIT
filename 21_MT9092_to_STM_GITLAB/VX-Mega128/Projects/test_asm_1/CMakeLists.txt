cmake_minimum_required(VERSION 3.5)

set(CMAKE_OBJCOPY /usr/bin/avr-objcopy)
set(CMAKE_OBJDUMP /usr/bin/avr-objdump)
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
add_executable(${PROJECT_NAME} ${src})

set(DISASS_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.disass)

#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#    COMMAND avr-as -mmcu=atmega128 ${CMAKE_SOURCE_DIR}/${nameFirmware}.asm -o ${CMAKE_BINARY_DIR}/${nameFirmware}.gnu_as
#    COMMENT "Assembler GNU ")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#    COMMAND ${CMAKE_OBJDUMP} -D $<TARGET_FILE:${PROJECT_NAME}> > ${DISASS_FILE}
    COMMAND ${CMAKE_OBJDUMP} -d $<TARGET_FILE:${PROJECT_NAME}> > ${DISASS_FILE}
    COMMENT "Disassemling to avr-gcc asm ${HEX_FILE} ")


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND cp ${CMAKE_SOURCE_DIR}/${nameFirmware}.asm ${CMAKE_BINARY_DIR}/${nameFirmware}.asm
    COMMENT "Copy asm to Build ${HEX_FILE} ")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#    COMMAND avra ${CMAKE_BINARY_DIR}/${nameFirmware}.asm
#    COMMAND ${CMAKE_SOURCE_DIR}/../gavrasm_lin_x86_64/gavrasm ${CMAKE_BINARY_DIR}/${nameFirmware}.asm
    COMMAND avr-as ${CMAKE_BINARY_DIR}/${nameFirmware}.asm
    COMMENT "Assembling ${HEX_FILE} ")

#add_custom_command(TARGET fake_c POST_BUILD
#    COMMAND avrdude -p m128 -c usbasp -B 4 -P usb -U flash:w:"${CMAKE_BINARY_DIR}/${nameFirmware}.hex":a
#    COMMENT "Write Flash ")




