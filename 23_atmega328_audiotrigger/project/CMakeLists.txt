cmake_minimum_required(VERSION 3.5)

set(CMAKE_OBJCOPY /usr/bin/avr-objcopy)

set(nameFirmware project)
project(${nameFirmware} LANGUAGES C)
add_definitions(-D__AVR_ATmega328p__)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}\
    -mmcu=atmega328p -DDEBUG -O1 -fdata-sections -ffunction-sections -fpack-struct -fshort-enums \
    -Wmissing-prototypes -Werror-implicit-function-declaration -Wpointer-arith  \
    -g3 -Wall -std=gnu99 -mrelax  -fno-strict-aliasing -Wstrict-prototypes \
    ")

add_executable(${nameFirmware} main.c)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C)
#set(LINKER_SCRIPT_FLASH "${CMAKE_SOURCE_DIR}/STM32F407VGTX_FLASH.ld")
#-T${LINKER_SCRIPT_FLASH} \
set(LINKER_FLAGS "SHELL: \
    -Wl,-Map=\"${PROJECT_NAME}.map\"
    -mmcu=atmega328p \
    -lc -lm \
    -Wl,--gc-sections,--print-memory-usage \
    ")
target_link_options(${nameFirmware} PRIVATE ${LINKER_FLAGS})


set(HEX_FILE ${CMAKE_BINARY_DIR}/${nameFirmware}.hex)
set(BIN_FILE ${CMAKE_BINARY_DIR}/${nameFirmware}.bin)

add_custom_command(TARGET ${nameFirmware} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${nameFirmware}> ${HEX_FILE}
    COMMENT "Building ${HEX_FILE} ")

add_custom_command(TARGET ${nameFirmware} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${nameFirmware}> ${BIN_FILE}
    COMMENT "Building ${BIN_FILE} ")

#add_custom_command(TARGET ${nameFirmware} POST_BUILD
#    COMMAND avrdude -p m328p -c usbasp -B 4 -P usb -U flash:w:"/home/evg/SOFT/Github/GIT/23_atmega328_audiotrigger/project/build/project.bin":a
#    COMMENT "Write Flash ")



