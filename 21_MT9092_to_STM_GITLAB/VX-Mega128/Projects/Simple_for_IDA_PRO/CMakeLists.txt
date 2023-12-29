cmake_minimum_required(VERSION 3.5)

set(CMAKE_OBJCOPY /usr/bin/avr-objcopy)
set(CMAKE_OBJDUMP /usr/bin/avr-objdump)
set(nameFirmware test_IDA)
project(${nameFirmware} LANGUAGES C)
add_definitions(-D__AVR_ATmega128__)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}\
    -mmcu=atmega128 -DDEBUG -O1 -fdata-sections -ffunction-sections -fpack-struct -fshort-enums \
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

add_custom_command(TARGET ${nameFirmware} POST_BUILD
    COMMAND ${CMAKE_OBJDUMP} -m avr:51 -d $<TARGET_FILE:${nameFirmware}> > ${nameFirmware}.dis
    COMMENT "Disassembling executable section ")

add_custom_command(TARGET ${nameFirmware} POST_BUILD
    COMMAND ${CMAKE_OBJDUMP} -m avr:51 -D $<TARGET_FILE:${nameFirmware}> > ${nameFirmware}.dis_all
    COMMENT "Disassembling all section ")

add_custom_command(TARGET ${nameFirmware} POST_BUILD
    COMMAND ${CMAKE_OBJDUMP} -m avr:51 -s $<TARGET_FILE:${nameFirmware}> > ${nameFirmware}.full_content
    COMMENT "Display the full contents of all sections ")

add_custom_command(TARGET ${nameFirmware} POST_BUILD
    COMMAND ${CMAKE_OBJDUMP} -m avr:51 -t $<TARGET_FILE:${nameFirmware}> > ${nameFirmware}.symb_tabs
    COMMENT "Display the contents of the symbol table(s) ")

add_custom_command(TARGET ${nameFirmware} POST_BUILD
    COMMAND avrdude -p m128 -c usbasp -B 10 -P usb -U flash:w:"/home/evg/SOFT/Github/GIT/21_MT9092_to_STM_GITLAB/VX-Mega128/Projects/Simple_for_IDA_PRO/build/${nameFirmware}.hex":a
    COMMENT "Write Flash ")

#avr-objdump -m avr:51 -D test_IDA.hex

