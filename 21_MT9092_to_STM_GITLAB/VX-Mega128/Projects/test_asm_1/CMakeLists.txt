#Светодиодная мигалка  ассемблер  gnu -  avr-as
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

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#    COMMAND ${CMAKE_OBJDUMP} -D $<TARGET_FILE:${PROJECT_NAME}> > ${DISASS_FILE}
    COMMAND ${CMAKE_OBJDUMP} -d $<TARGET_FILE:${PROJECT_NAME}> > ${DISASS_FILE}
    COMMENT "Disassemling c to avr-gcc asm ${HEX_FILE} ")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND cp ${CMAKE_SOURCE_DIR}/${nameFirmware}.s ${CMAKE_BINARY_DIR}/${nameFirmware}.s
    COMMENT "Copy asm to Build ${HEX_FILE} ")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND avr-as -mmcu=atmega128 -o ${CMAKE_BINARY_DIR}/${nameFirmware}.o ${CMAKE_BINARY_DIR}/${nameFirmware}.s
    COMMENT "Assembling ")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND avr-ld -o ${CMAKE_BINARY_DIR}/${nameFirmware}.elf ${CMAKE_BINARY_DIR}/${nameFirmware}.o -g
    -mavr51 COMMENT "Linking ")

#You must add the desired architecture with the option -m, for example:
#avr-ld  -o main.elf main.o  -g -mavr51
#If this does not work you get following error:
#avr-ld: cannot open linker script file ldscripts/avr51.xn: No such file or directory
#You must force using the right linkerscript with the -T option:
#avr-ld  -o main.elf main.o  -g -mavr51 -T /usr/lib/avr/ldscripts/avr51.xn

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND avr-objcopy --output-target=ihex ${CMAKE_BINARY_DIR}/${nameFirmware}.elf ${CMAKE_BINARY_DIR}/${nameFirmware}.hex
    COMMENT "Create HEX ")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND avr-objdump -m avr -D ${CMAKE_BINARY_DIR}/${nameFirmware}.hex >  ${CMAKE_BINARY_DIR}/${nameFirmware}.dis
    COMMENT "Disassemling control ")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND avrdude -p m128 -c usbasp -B 4 -P usb -U flash:w:"${CMAKE_BINARY_DIR}/${nameFirmware}.hex":a
    COMMENT "Write Flash ")

