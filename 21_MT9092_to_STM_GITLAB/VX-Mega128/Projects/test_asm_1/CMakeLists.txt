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

#set(HEX_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.hex)
#set(BIN_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET fake_c POST_BUILD
    COMMAND avra ${CMAKE_SOURCE_DIR}/blink.asm
    COMMENT "Building ${HEX_FILE} ")

#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}> ${HEX_FILE}
#    COMMENT "Building ${HEX_FILE} ")


#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}> ${HEX_FILE}
#    COMMENT "Building ${HEX_FILE} ")

#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}> ${BIN_FILE}
#    COMMENT "Building ${BIN_FILE} ")

add_custom_command(TARGET fake_c POST_BUILD
    COMMAND avrdude -p m128 -c usbasp -B 4 -P usb -U flash:w:"/home/evg/SOFT/Github/GIT/21_MT9092_to_STM_GITLAB/VX-Mega128/Projects/test_asm_1/blink.hex":a
    COMMENT "Write Flash ")




