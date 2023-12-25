cmake_minimum_required(VERSION 3.5)

set(CMAKE_OBJCOPY /usr/bin/avr-objcopy)

set(nameFirmware DX_SPI)
project(${nameFirmware} LANGUAGES C)
add_definitions(-D__AVR_ATmega128__)
#INCLUDE(generic-gcc-avr.cmake)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}\
    -mmcu=atmega128 -DDEBUG -O3 -fdata-sections -ffunction-sections -fpack-struct -fshort-enums \
    -Wmissing-prototypes -Werror-implicit-function-declaration -Wpointer-arith  \
    -g3 -Wall -std=gnu99 -mrelax  -fno-strict-aliasing -Wstrict-prototypes -Wl,-u,vfprintf -lprintf_flt -lm \
    ")

include_directories(Inc /usr/lib/avr/lib)
file(GLOB Src Src/*.c)
add_executable(${PROJECT_NAME} ${Src})

#find_library(C_LIB c)
#message(STATUS "avr-libc: ${C_LIB}")
#target_link_libraries(${PROJECT_NAME} ${C_LIB})
#target_link_libraries(${PROJECT_NAME} -L/usr/lib/avr/lib/avr51/libprintf_min.a)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C)
#set(LINKER_SCRIPT_FLASH "${CMAKE_SOURCE_DIR}/STM32F407VGTX_FLASH.ld")
#-T${LINKER_SCRIPT_FLASH} \
set(LINKER_FLAGS "SHELL: \
    -Wl,-Map=\"${PROJECT_NAME}.map\"
    -mmcu=atmega128 \
    -lc -lm \
    -Wl,--gc-sections,--print-memory-usage \
    ")
#    -Wl,-u,vfprintf \
#-lprintf_min -lscanf_min \
target_link_options(${nameFirmware} PRIVATE ${LINKER_FLAGS})



set(HEX_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${nameFirmware} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${nameFirmware}> ${HEX_FILE}
    COMMENT "Building ${HEX_FILE} ")

add_custom_command(TARGET ${nameFirmware} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${nameFirmware}> ${BIN_FILE}
    COMMENT "Building ${BIN_FILE} ")

add_custom_command(TARGET ${nameFirmware} POST_BUILD
    COMMAND avrdude -p m128 -B 20 -c jtag2isp -P usb -U flash:w:"/home/evg/SOFT/Github/GIT/21_MT9092_to_STM_GITLAB/VX-Mega128/Projects/DX_SPI/build/${nameFirmware}.hex":a
    COMMENT "Write Flash ")



