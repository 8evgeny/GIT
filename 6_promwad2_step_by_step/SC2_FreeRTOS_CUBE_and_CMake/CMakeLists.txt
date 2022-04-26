cmake_minimum_required(VERSION 3.15)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Set ARM compiler
set(TOOLS_PATH /home/evg/toolchain/gcc-arm-none-eabi-new)
#set(CMAKE_C_COMPILER ${TOOLS_PATH}/bin/arm-none-eabi-gcc)
#set(CMAKE_CXX_COMPILER ${TOOLS_PATH}/bin/arm-none-eabi-g++)
set(CMAKE_CXX_LINKER_LAUNCHER ${TOOLS_PATH}/bin/arm-none-eabi-g++)
set(CMAKE_OBJCOPY ${TOOLS_PATH}/bin/arm-none-eabi-objcopy)

project(FreeRTOS_STMH753 C CXX ASM)

add_definitions(-DSTM32H753xx)
add_definitions(-DUSE_HAL_DRIVER)
add_definitions(-D__weak=__attribute__\(\(weak\)\))
add_definitions(-D__packed=__attribute__\(\(__packed__\)\))
add_definitions(-D__FPU_PRESENT=1)
add_definitions(-DARM_MATH_CM7)
add_definitions(-DHAVE_CONFIG_H)
add_definitions(-D__DCACHE_PRESENT)
add_definitions(-D__ICACHE_PRESENT)
add_definitions(-DSC2BOARD)

#include "stm32h753xx.h"  // все регистры

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}\
    -Wl,--start-group \
    -Wl,--gc-sections \
    -mcpu=cortex-m7 \
    -DUSE_HAL_DRIVER \
    -DSTM32H753xx \
    -Os \
    -ffunction-sections \
    -fdata-sections \
    -Wall \
    -mno-unaligned-access \
    -fstack-usage \
    -MMD \
    -MP \
    -mfpu=fpv5-d16 \
    -mfloat-abi=hard \
    -mthumb \
    -Xlinker \
    -Map=${PROJECT_NAME}.map \
    -Wl,-section-start=.rtt=0x20000000 \
    -u_printf_float \
    -fno-exceptions \
    -Wl,--end-group \
    ")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}\
    -Wl,--start-group \
    -Wl,--gc-sections \
    -mcpu=cortex-m7 \
    -DUSE_HAL_DRIVER \
    -DSTM32H753xx \
    -Os \
    -ffunction-sections \
    -fdata-sections \
    -Wall \
    -mno-unaligned-access \
    -fstack-usage \
    -MMD \
    -MP \
    -mfpu=fpv5-d16 \
    -mfloat-abi=hard \
    -mthumb \
    -Xlinker \
    -Map=${PROJECT_NAME}.map \
    -Wl,-section-start=.rtt=0x20000000 \
    -u_printf_float \
    -fno-exceptions \
    -Wl,--end-group \
    ")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}\
    -mcpu=cortex-m7 \
    -std=gnu11 \
    -DUSE_HAL_DRIVER \
    -DSTM32H753xx \
    -Os \
    -ffunction-sections \
    -fdata-sections \
    -Wall \
    -mno-unaligned-access \
    -fstack-usage \
    -MMD \
    -MP \
    -mfpu=fpv5-d16 \
    -mfloat-abi=hard \
    -mthumb \
    -Xlinker -Map=${PROJECT_NAME}.map \
    ")

include_directories(Inc)
file (GLOB SOURCES_C Src/*.c STM32H753IITX_FLASH.ld)
file (GLOB SOURCES_CPP Src/*.cpp)

include_directories(
    Drivers/CMSIS/Include
    Drivers/CMSIS/Device/ST/STM32H7xx/Include
    Drivers/BSP/Components/dp83848
    )
file (GLOB ETH_SOURCES
    LWIP/Target/ethernetif.c
    )
add_library(ETH ${ETH_SOURCES} )


set (RS232  Src/RS232)
include_directories( ${RS232} )
file (GLOB RS232_src ${RS232}/*.cpp ${RS232}/*.c ${RS232}/*.h )
add_library(RS232_LIB ${RS232_src} )

set (Debug  Src/Debug)
include_directories( ${Debug} )
file (GLOB Debug_src ${Debug}/*.cpp ${Debug}/*.c ${Debug}/*.h )
add_library(Debug_LIB ${Debug_src} )

set (EEPROM  Src/EEPROM)
include_directories(${EEPROM})
file (GLOB EEPROM_src ${EEPROM}/*.cpp ${EEPROM}/*.c ${EEPROM}/*.h )
add_library(EEPROM_LIB ${EEPROM_src} )

set (littlefs Middlewares/Third_Party/littlefs)
include_directories( ${littlefs} )
file (GLOB littlefs_src ${littlefs}/*.c ${littlefs}/*.h  )
add_library(littlefs_LIB ${littlefs_src} )

set (CircularBuffer Middlewares/Third_Party/CircularBuffer)
include_directories( ${CircularBuffer} )
file (GLOB CircularBuffer_src ${CircularBuffer}/*.tpp  ${CircularBuffer}/*.h )
add_library(CircularBuffer_LIB ${CircularBuffer_src} )
set_target_properties(CircularBuffer_LIB PROPERTIES LINKER_LANGUAGE CXX)

set (SRAM Src/SRAM)
include_directories( ${SRAM} )
file (GLOB SRAM_src ${SRAM}/*.cpp ${SRAM}/*.h  )
add_library(SRAM_LIB ${SRAM_src} )

set (JSON  Src/JSON)
include_directories( ${JSON} )
file (GLOB JSON_src ${JSON}/*.cpp ${JSON}/*.h )
add_library(JSON_LIB ${JSON_src} )

set (ArduinoJson Middlewares/Third_Party/ArduinoJson)
include_directories( ${ArduinoJson} Middlewares/Third_Party/ArduinoJson/src )

set (RTC  Src/RTC)
include_directories( ${RTC} )
file (GLOB RTC_src ${RTC}/*.cpp ${RTC}/*.h )
add_library(RTC_LIB ${RTC_src} )

set (SNTP  Src/SNTP)
include_directories( ${SNTP} )
file (GLOB SNTP_src ${SNTP}/*.cpp ${SNTP}/*.h )
add_library(SNTP_LIB ${SNTP_src} )

set (TRNG  Src/TRNG)
include_directories( ${TRNG} )
file (GLOB TRNG_src ${TRNG}/*.cpp ${TRNG}/*.h )
add_library(TRNG_LIB ${TRNG_src} )

set (GPIO_SC2  Src/GPIO_SC2_BOARD)
include_directories( ${GPIO_SC2} )
file (GLOB GPIO_SC2_SOURCES ${GPIO_SC2}/*.cpp ${GPIO_SC2}/*.h)
add_library(GPIO_SC2_LIB ${GPIO_SC2_SOURCES} )

set (UpdateMechanism  Src/UpdateMechanism)
include_directories( ${UpdateMechanism} )
file (GLOB UpdateMechanism_src ${UpdateMechanism}/*.cpp ${UpdateMechanism}/*.h )
add_library(UpdateMechanism_LIB ${UpdateMechanism_src} )

set (TraceRecorder  "Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder")
include_directories(${TraceRecorder}/streamports/TCPIP/include ${TraceRecorder}/include ${TraceRecorder}/config )
file (GLOB TraceRecorder_src ${TraceRecorder}/*.c ${TraceRecorder}/streamports/TCPIP/*.c ${TraceRecorder}/streamports/TCPIP/include/*.h ${TraceRecorder}/include/*.h ${TraceRecorder}/config/*.h )
add_library(TraceRecorder_LIB ${TraceRecorder_src} )

set (RTP  Src/RTP)
include_directories( ${RTP} Drivers/CMSIS/DSP/Include)
file (GLOB RTP_src ${RTP}/*.cpp ${RTP}/*.h )
add_library(RTP_LIB ${RTP_src} )

set (AUDIO  Src/AUDIO)
include_directories(
    ${AUDIO}
    Drivers/CMSIS/DSP/Include
    )
file (GLOB AUDIO_src ${AUDIO}/*.cpp ${AUDIO}/*.h )
add_library(AUDIO_LIB ${AUDIO_src} )

set (UDP_JSON  Src/UDP_JSON)
include_directories( ${UDP_JSON} )
file (GLOB UDP_JSON_src ${UDP_JSON}/*.cpp ${UDP_JSON}/*.h )
add_library(UDP_JSON_LIB ${UDP_JSON_src} )

set (Call_Control  Src/Call_Control)
include_directories( ${Call_Control} )
file (GLOB Call_Control_src ${Call_Control}/*.cpp ${Call_Control}/*.h )
list(REMOVE_ITEM Call_Control_src ${Call_Control}/call_control.h ${Call_Control}/call_control.cpp )
add_library(Call_Control_LIB ${Call_Control_src} )

set (flash  Src/flash)
include_directories( ${flash} )
file (GLOB flash_src ${flash}/*.cpp ${flash}/*.h )
add_library(flash_LIB ${flash_src} )

set (FatFs Middlewares/Third_Party/FatFs)
include_directories(${FatFs}/src )
file (GLOB FatFs_src ${FatFs}/src/*.c ${FatFs}/src/option/syscall.c ${FatFs}/src/option/unicode.c ${FatFs}/src/*.h )
add_library(FatFs_LIB ${FatFs_src} )

set (Tests  Src/Tests)
include_directories( ${Tests} )
file (GLOB Tests_src ${Tests}/*.cpp ${Tests}/*.h )
add_library(Tests_LIB ${Tests_src} )

set (uid  Src/uid)
include_directories( ${uid} )
file (GLOB uid_src ${uid}/*.cpp ${uid}/*.h )
add_library(uid_LIB ${uid_src} )

set (WDT  Src/WDT)
include_directories( ${WDT} )
file (GLOB WDT_src ${WDT}/*.cpp ${WDT}/*.h )
add_library(WDT_LIB ${WDT_src} )

set (STARTUP_SOURCES startup/startup_stm32h753iitx.s )
add_library(STARTUP ${STARTUP_SOURCES} )

set (BSP_SOURCES Drivers/BSP/Components/lan8742/lan8742.c )
include_directories( Drivers/BSP/Components/lan8742 )
add_library(BSP ${BSP_SOURCES} )

file (GLOB HAL_SRC Drivers/STM32H7xx_HAL_Driver/Src/*)
include_directories(Drivers/STM32H7xx_HAL_Driver/Inc  Drivers/STM32H7xx_HAL_Driver/Inc/Legacy )
add_library(STM32H7xx_HAL_Driver ${HAL_SRC})

file (GLOB CMSIS_RTOS_SOURCES
    Middlewares/Third_Party/FreeRTOS/Source/*.c
    Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c
    Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
    Middlewares/Third_Party/LwIP/src/core/*.c
    Middlewares/Third_Party/LwIP/system/OS/*.c
    Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/*.c
    )
include_directories(
    Middlewares/Third_Party/FreeRTOS/Source/include
    Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS
    Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang
    Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
    )
add_library(CMSIS_RTOS ${CMSIS_RTOS_SOURCES} )

set (LWIP_SOURCES LWIP/App/lwip.c  LWIP/Target/ethernetif.c LWIP/ping.c )
include_directories(LWIP/App LWIP/Target LWIP )
add_library(LWIP ${LWIP_SOURCES} )

file (GLOB Third_Party_SRC
    Middlewares/Third_Party/LwIP/src/api/*
    Middlewares/Third_Party/LwIP/src/apps/mqtt/mqtt.c
    Middlewares/Third_Party/LwIP/src/core/*
    Middlewares/Third_Party/LwIP/src/core/ipv4/*
    Middlewares/Third_Party/LwIP/src/netif/*
    Middlewares/Third_Party/LwIP/src/netif/ppp/*
    )
include_directories(
    Middlewares/Third_Party/LwIP/system/arch
    Middlewares/Third_Party/LwIP/system
    Middlewares/Third_Party/LwIP/src/include/netif
    Middlewares/Third_Party/LwIP/src/include/netif/ppp
    Middlewares/Third_Party/LwIP/src/include
    Middlewares/Third_Party/LwIP/src/include/lwip
    Middlewares/Third_Party/LwIP/src/include/lwip/apps
    Middlewares/Third_Party/LwIP/src/include/lwip/priv
    Middlewares/Third_Party/LwIP/src/include/lwip/prot
    Middlewares/Third_Party/LwIP/src/include/compat/posix
    Middlewares/Third_Party/LwIP/src/include/compat/arpa
    Middlewares/Third_Party/LwIP/src/include/compat/net
    Middlewares/Third_Party/LwIP/src/include/compat/sys
    )
add_library(Middlewares ${Third_Party_SRC} )


add_executable(${PROJECT_NAME}.elf  ${SOURCES_C}  ${SOURCES_CPP} )


set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINKER_LANGUAGE CXX)
set(LINKER_SCRIPT_FLASH "${CMAKE_SOURCE_DIR}/STM32H753IITX_FLASH.ld")
set(LINKER_SCRIPT_RAM "${CMAKE_SOURCE_DIR}/STM32H753IITX_RAM.ld")
set(LINKER_FLAGS "SHELL: \
    -T${LINKER_SCRIPT_FLASH} \
    -mcpu=cortex-m7 \
    -lstdc++ \
    --specs=nosys.specs \
    -Wl,--gc-sections \
    -static \
    --specs=nano.specs \
    -mfpu=fpv5-d16 \
    -mfloat-abi=hard \
    -mthumb
    -Wl,--start-group \
    -lc \
    -lm \
    -Wl,--end-group \
    ")
#    -T${LINKER_SCRIPT_RAM} \

target_link_options(${PROJECT_NAME}.elf PRIVATE ${LINKER_FLAGS})

target_link_libraries(${PROJECT_NAME}.elf PUBLIC
    ETH
    STARTUP
    BSP
    STM32H7xx_HAL_Driver
    LWIP
    Middlewares
    CMSIS_RTOS
    RS232_LIB
    Debug_LIB
    EEPROM_LIB
    littlefs_LIB
    CircularBuffer_LIB
    SRAM_LIB
    JSON_LIB
    RTC_LIB
    SNTP_LIB
    TRNG_LIB
    GPIO_SC2_LIB
    UpdateMechanism_LIB
    TraceRecorder_LIB
    RTP_LIB
    AUDIO_LIB
    UDP_JSON_LIB
    Call_Control_LIB
    flash_LIB
    FatFs_LIB
    Tests_LIB
    uid_LIB
    WDT_LIB
    )

set(HEX_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin)
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
    COMMENT "Building ${HEX_FILE} ")
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
    COMMENT "Building ${BIN_FILE} ")


