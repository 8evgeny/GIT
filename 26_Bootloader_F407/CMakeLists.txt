cmake_minimum_required(VERSION 3.15)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Set ARM compiler
set(toolchain /home/evg/toolchain/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi)
#set(TOOLS_PATH /home/evg/toolchain/gcc-arm-none-eabi-new)
#set(CMAKE_C_COMPILER ${toolchain}/bin/arm-none-eabi-gcc)
#set(CMAKE_CXX_COMPILER ${toolchain}/bin/arm-none-eabi-g++)
#set(CMAKE_CXX_LINKER_LAUNCHER ${toolchain}/bin/arm-none-eabi-g++)
#set(CMAKE_OBJCOPY ${toolchain}/bin/arm-none-eabi-objcopy)
set(CMAKE_OBJCOPY ${toolchain}/bin/arm-none-eabi-objcopy)

#set(LFS ON)
add_definitions(-DDATA_IN_CCM)      #Разрешение data в CCM
#add_definitions(-Denable_write_MAC) #Разрешение технологической прошивки / записи MAC
#add_definitions(-Denable_twin_reset_to_restore_default_IP)
add_definitions(-Denable_SEGGER) #on/off SEGGER
add_definitions(-DNEW_HTTP_SERVER) #Основной сервер WIZNET или пример из инета
#add_definitions(-DTLS_ON)         #Режим http / https
add_definitions(-DEEPROMADRESS8bit) #Если EEPROM i2c большая (адрес 16 бит - строчку закомментировать)
#add_definitions(-DMAC_IN_DECIMAL) #MAC в 16 или 10 виде
add_definitions(-DPROD)  #Основной файл index.html или пример WIZNET

#TЕСТОВАЯ  ОТПРАВКА  ДАННЫХ
#add_definitions(-DbaseSendTestData)  #Отправка c базы тестовых данных абоненту
#add_definitions(-DabonSendTestData)  #Отправка от абонента тестовых данных в базу
#add_definitions(-DfpgaToCpuBaseTestData)  #Отдаю из FPGA в CPU тестовые данные на базе
#add_definitions(-DfpgaToCpuAbonTestData)  #Отдаю из FPGA в CPU тестовые данные на абоненте
#add_definitions(-DcpuToFpgaBaseTestData)  #Отдаю из CPU в FPGA тестовые данные на базе
#add_definitions(-DcpuToFpgaAbonTestData)  #Отдаю из CPU в FPGA тестовые данные на абоненте
add_definitions(-DWOLFSSL_X509_INCLUDED)
add_definitions(-DOPENSSL_ALL)
add_definitions(-UWOLFSSL_TLS13)

project(Bootloader C CXX ASM)

FILE(STRINGS "version_Bootloader" tmp LIMIT_COUNT 2)
STRING(REGEX MATCHALL "[a-zA-Z0-9_]+[ ]*=[^\n]*(\n|$)" tmp_list "${tmp}")
FOREACH(tmp_x ${tmp_list})
        STRING(REGEX REPLACE "([a-zA-Z0-9_]+)[ ]*=[^\n]*(\n|$)" "\\1" tmp_nm "${tmp_x}")
        STRING(REGEX REPLACE "[a-zA-Z0-9_]+[ ]*=[ ]*([^\n]*)(\n|$)" "\\1" tmp_val "${tmp_x}")
        STRING(STRIP "${tmp_val}" ${tmp_nm})
ENDFOREACH()
MESSAGE("Version firmware: ${MAIN_VERSION_FIRMWARE}.${PATCH_VERSION}")
set (MAIN_FW "${MAIN_VERSION_FIRMWARE}")
set (PATCH_FW "${PATCH_VERSION}")
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc/config.h.in config.h @ONLY)


set (ioLibrary_Eth ${CMAKE_HOME_DIRECTORY}/ioLibrary_Driver/Ethernet)
set (ioLibrary_Int ${CMAKE_HOME_DIRECTORY}/ioLibrary_Driver/Internet/httpServer)
set (fatfs ${CMAKE_HOME_DIRECTORY}/Middlewares/Third_Party/FatFs/src)

if(LFS STREQUAL ON)
    add_definitions(-DLFS)
    #set (littlefs ${CMAKE_HOME_DIRECTORY}/Middlewares/Third_Party/littlefs_new)
    set (littlefs ${CMAKE_HOME_DIRECTORY}/Middlewares/Third_Party/littlefs)
    include_directories(${littlefs})
endif()

#SET(MYCOMPILE_FLAGS '-DMBEDTLS_CONFIG_FILE="mbedtls_config.h"') #для mbedTLS
add_definitions(${MYCOMPILE_FLAGS})
add_definitions("-DUSE_FULL_LL_DRIVER")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}\
    -mcpu=cortex-m4 \
    -std=gnu11 \
    -DUSE_HAL_DRIVER \
    -DSTM32F407xx \
    -c \
    -Os \
    -g \
    -ffunction-sections \
    -fdata-sections \
    -Wall \
    -fstack-usage \
    -mfpu=fpv4-sp-d16 \
    -mfloat-abi=hard \
    -mthumb \
    -Xlinker -Map=${PROJECT_NAME}.map \
    ")
#    --specs=nano.specs \

include_directories(
    build
    Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
    Core/Inc
    Drivers/CMSIS/Device/ST/STM32F4xx/Include
    Drivers/STM32F4xx_HAL_Driver/Inc
    Drivers/CMSIS/Core/Include
    Drivers/CMSIS/Include
    ${ioLibrary_Eth}
    ${ioLibrary_Int}
    FATFS/App
    FATFS/Target
    ${fatfs}
    RTT
    )

file (GLOB hal_src Drivers/STM32F4xx_HAL_Driver/Src/*.c)
add_library(hal_LIB ${hal_src})

file (GLOB ioLibrary_Driver_src
    ${ioLibrary_Eth}/*.c
    ${ioLibrary_Eth}/W5500/*.c
    ${ioLibrary_Int}/*.c
    )

#file (GLOB_RECURSE bear_src ${bearssl}/src/*.c  ioLibrary_Driver/Internet/httpServer/*.c)
#add_library(bearSSL_LIB ${bear_src} )

file (GLOB fatfs_src FATFS/App/*.c FATFS/Target/*.c ${fatfs}/*.c ${fatfs}/option/*.c)
add_library(fatfs_LIB ${fatfs_src})

if(LFS STREQUAL ON)
    file (GLOB littlefs_src ${littlefs}/*.c ${littlefs}/*.h)
    add_library(littlefs_LIB ${littlefs_src})
endif()

file (GLOB wolfssl_src ${wolfssl}/src/*.c
    ${wolfssl}/wolfcrypt/src/*.c
    wolfSSL/tls-server-size.c
    )

#file (GLOB polarssl_src ${polarssl}/*.c
#    ${polarssl}/mbedtls/library/*.c
#    )
#add_library(polarssl_LIB ${polarssl_src})

file (GLOB src Core/Src/*.c Core/Startup/startup_stm32f407vgtx.s Middlewares/Third_Party/littlefs/eeprom.c)

set(nameFirmware ${PROJECT_NAME}_${MAIN_VERSION_FIRMWARE}_${PATCH_VERSION})
add_executable(${nameFirmware}
    ${src}
    )

target_compile_definitions(${nameFirmware} PRIVATE fwareName=${nameFirmware})

set_target_properties(${nameFirmware} PROPERTIES LINKER_LANGUAGE CXX)
set(LINKER_SCRIPT_FLASH "${CMAKE_SOURCE_DIR}/STM32F407VGTX_FLASH.ld")
set(LINKER_FLAGS "SHELL: \
    -T${LINKER_SCRIPT_FLASH} \
    -mcpu=cortex-m4 \
    --specs=nosys.specs \
    -Wl,-Map=${nameFirmware}.map \
    -Wl,--gc-sections \
    -static \
    --specs=nano.specs \
    -Wl,--gc-sections,--print-memory-usage \
    -mfpu=fpv5-d16 \
    -mfloat-abi=hard \
    -mthumb
    -Wl,--start-group \
    -lc \
    -lm \
    -Wl,--end-group \
    ")

target_link_options(${nameFirmware} PRIVATE ${LINKER_FLAGS})

target_link_libraries(${nameFirmware} PUBLIC
    hal_LIB
    fatfs_LIB
    )

if(LFS STREQUAL ON)
    target_link_libraries(${nameFirmware} PUBLIC littlefs_LIB )
endif()

set(HEX_FILE ${CMAKE_BINARY_DIR}/${nameFirmware}.hex)
set(BIN_FILE ${CMAKE_BINARY_DIR}/${nameFirmware}.bin)

add_custom_command(TARGET ${nameFirmware} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${nameFirmware}> ${HEX_FILE}
    COMMENT "Building ${HEX_FILE} ")

add_custom_command(TARGET ${nameFirmware} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${nameFirmware}> ${BIN_FILE}
    COMMENT "Building ${BIN_FILE} ")

#add_custom_command(TARGET ${nameFirmware} POST_BUILD
#    COMMAND st-flash erase
#    COMMENT "Erasing . . .")

#add_custom_command(TARGET ${nameFirmware} POST_BUILD
#    COMMAND st-flash --reset write ${BIN_FILE} 0x8000000
#    COMMENT "Write Flash ")
