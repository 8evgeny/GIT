#Светодиодная мигалка  ассемблер  gnu -  avr-as  Сборка через Makefile
#https://count-zero.ru/2018/gnu_assembler/
cmake_minimum_required(VERSION 3.5)

set(CMAKE_OBJCOPY /usr/bin/avr-objcopy)
set(CMAKE_OBJDUMP /usr/bin/avr-objdump)
set(nameFirmware test_asm_2)
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
set(DISASS_FILE_ASM ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.dis)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJDUMP} -d $<TARGET_FILE:${PROJECT_NAME}> > ${DISASS_FILE}
    COMMENT "Disassemling c to avr-gcc asm ${HEX_FILE} ")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND cp ${CMAKE_SOURCE_DIR}/Makefile ${CMAKE_BINARY_DIR}/Makefile
    COMMENT "replace Makefile")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND cp ${CMAKE_SOURCE_DIR}/${nameFirmware}.S ${CMAKE_BINARY_DIR}/${nameFirmware}.S
    COMMENT "copy SRC")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND make all
    COMMENT "make all")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJDUMP} -m avr -D ${CMAKE_BINARY_DIR}/${nameFirmware}.hex > ${DISASS_FILE_ASM}
    COMMENT "Disassemling control ")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND make install
    COMMENT "make install ")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND make clean
    COMMENT "make clean ")

