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
#    -fno-rtti \
#    -std=gnu11 \



#set(CMAKE_BUILD_TYPE debug)
#set(CMAKE_BUILD_TYPE release)

set (patchSRC "Src")
set (patchDRV "Drivers")
set (patchHAL_SRC "${patchDRV}/STM32H7xx_HAL_Driver/Src")

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
file (GLOB SOURCES_C ${patchSRC}/*.c STM32H753IITX_FLASH.ld)

file (GLOB SOURCES_CPP ${patchSRC}/*.cpp)

include_directories(
    ${patchDRV}/CMSIS/Include
    ${patchDRV}/CMSIS/Device/ST/STM32H7xx/Include
    Drivers/BSP/Components/dp83848
    )
file (GLOB CORE_SOURCES
    Src/*.c
    Drivers/BSP/Components/dp83848/dp83848.c
    LWIP/Target/ethernetif.c
    )
add_library(CORE ${CORE_SOURCES} )





set (RS232  ${patchSRC}/RS232)
include_directories( ${RS232} )
file (GLOB RS232_src ${RS232}/*.cpp ${RS232}/*.c ${RS232}/*.h )
add_library(RS232_LIB ${RS232_src} )

set (Debug  ${patchSRC}/Debug)
include_directories( ${Debug} )
file (GLOB Debug_src ${Debug}/*.cpp ${Debug}/*.c ${Debug}/*.h )
add_library(Debug_LIB ${Debug_src} )

set (EEPROM  ${patchSRC}/EEPROM)
include_directories(${EEPROM})
file (GLOB EEPROM_src ${EEPROM}/*.cpp ${EEPROM}/*.c ${EEPROM}/*.h )
add_library(EEPROM_LIB ${EEPROM_src} )

set (littlefs Middlewares/Third_Party/littlefs)
include_directories( ${littlefs} )
file (GLOB littlefs_src ${littlefs}/*.c ${littlefs}/*.h  )
add_library(littlefs_LIB ${littlefs_src} )

set (CircularBuffer Middlewares/Third_Party/CircularBuffer)
include_directories( ${CircularBuffer} Src/c-ringbuf )
file (GLOB CircularBuffer_src ${CircularBuffer}/*.tpp  ${CircularBuffer}/*.h )
add_library(CircularBuffer_LIB ${CircularBuffer_src} )
set_target_properties(CircularBuffer_LIB PROPERTIES LINKER_LANGUAGE CXX)

set (SRAM ${patchSRC}/SRAM)
include_directories( ${SRAM} )
file (GLOB SRAM_src ${SRAM}/*.cpp ${SRAM}/*.h  )
add_library(SRAM_LIB ${SRAM_src} )

set (JSON  ${patchSRC}/JSON)
include_directories( ${JSON} )
file (GLOB JSON_src ${JSON}/*.cpp ${JSON}/*.h )
add_library(JSON_LIB ${JSON_src} )

set (ArduinoJson Middlewares/Third_Party/ArduinoJson)
include_directories( ${ArduinoJson} Middlewares/Third_Party/ArduinoJson/src )

set (RTC  ${patchSRC}/RTC)
include_directories( ${RTC} )
file (GLOB RTC_src ${RTC}/*.cpp ${RTC}/*.h )
add_library(RTC_LIB ${RTC_src} )

set (SNTP  ${patchSRC}/SNTP)
include_directories( ${SNTP} )
file (GLOB SNTP_src ${SNTP}/*.cpp ${SNTP}/*.h )
add_library(SNTP_LIB ${SNTP_src} )

set (TRNG  ${patchSRC}/TRNG)
include_directories( ${TRNG} )
file (GLOB TRNG_src ${TRNG}/*.cpp ${TRNG}/*.h )
add_library(TRNG_LIB ${TRNG_src} )

#set (GPIO_SC2  ${patchSRC}/GPIO_SC2_BOARD)
#include_directories( ${GPIO_SC2} )
#file (GLOB GPIO_SC2_SOURCES ${GPIO_SC2}/*.cpp ${GPIO_SC2}/*.h)
#add_library(GPIO_SC2_LIB ${GPIO_SC2_SOURCES} )

#set (AUDIO  ${patchSRC}/AUDIO)
#include_directories(
#    ${AUDIO}
#    Drivers/CMSIS/DSP/Include
#    )
#file (GLOB AUDIO_src ${AUDIO}/*.cpp ${AUDIO}/*.h )
#add_library(AUDIO_LIB ${AUDIO_src} )

#set (RTP  ${patchSRC}/RTP)
#include_directories( ${RTP} )
#file (GLOB RTP_src ${RTP}/*.cpp ${RTP}/*.h )
#add_library(RTP_LIB ${RTP_src} )

#set (UDP_JSON  ${patchSRC}/UDP_JSON)
#include_directories( ${UDP_JSON} )
#file (GLOB UDP_JSON_src ${UDP_JSON}/*.cpp ${UDP_JSON}/*.h )
#add_library(UDP_JSON_LIB ${UDP_JSON_src} )

#set (Call_Control  ${patchSRC}/Call_Control)
#include_directories( ${Call_Control} )
#file (GLOB Call_Control_src ${Call_Control}/*.cpp ${Call_Control}/*.h )
#list(REMOVE_ITEM Call_Control_src ${Call_Control}/call_control.h ${Call_Control}/call_control.cpp )
#add_library(Call_Control_LIB ${Call_Control_src} )

#set (Call_control_SC2  ${patchSRC}/Call_control_SC2)
#include_directories( ${Call_control_SC2} )
#file (GLOB Call_Control_SC2_src ${Call_control_SC2}/*.cpp ${Call_control_SC2}/*.h )
#add_library(Call_Control_SC2_LIB ${Call_Control_SC2_src} )

#set (flash  ${patchSRC}/flash)
#include_directories( ${flash} )
#file (GLOB flash_src ${flash}/*.cpp ${flash}/*.h )
#add_library(flash_LIB ${flash_src} )

#set (FatFs Middlewares/Third_Party/FatFs)
#include_directories(${FatFs}/src )
#file (GLOB FatFs_src ${FatFs}/src/*.c ${FatFs}/src/option/syscall.c ${FatFs}/src/option/unicode.c ${FatFs}/src/*.h )
#add_library(FatFs_LIB ${FatFs_src} )

#set (Tests  ${patchSRC}/Tests)
#include_directories( ${Tests} )
#file (GLOB Tests_src ${Tests}/*.cpp ${Tests}/*.h )
#add_library(Tests_LIB ${Tests_src} )

#set (uid  ${patchSRC}/uid)
#include_directories( ${uid} )
#file (GLOB uid_src ${uid}/*.cpp ${uid}/*.h )
#add_library(uid_LIB ${uid_src} )

#set (WDT  ${patchSRC}/WDT)
#include_directories( ${WDT} )
#file (GLOB WDT_src ${WDT}/*.cpp ${WDT}/*.h )
#add_library(WDT_LIB ${WDT_src} )







set (STARTUP_SOURCES startup/startup_stm32h753iitx.s )
add_library(STARTUP ${STARTUP_SOURCES} )

include_directories( ${patchDRV}/BSP/Components/lan8742 )
set (BSP_SOURCES ${patchDRV}/BSP/Components/lan8742/lan8742.c )
add_library(BSP ${BSP_SOURCES} )

include_directories(
    ${patchDRV}/STM32H7xx_HAL_Driver/Inc
    ${patchDRV}/STM32H7xx_HAL_Driver/Inc/Legacy
    )
file (GLOB HAL_SRC ${patchHAL_SRC}/*)
add_library(STM32H7xx_HAL_Driver ${HAL_SRC})


include_directories(
    Middlewares/Third_Party/FreeRTOS/Source/include
    Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS
    Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang
    Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
    )
file (GLOB CMSIS_RTOS_SOURCES
    Middlewares/Third_Party/FreeRTOS/Source/*.c
    Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c
    Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
    Middlewares/Third_Party/LwIP/src/core/*.c
    Middlewares/Third_Party/LwIP/system/OS/*.c
    Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/*.c
    )

add_library(CMSIS_RTOS ${CMSIS_RTOS_SOURCES} )


include_directories(
    LWIP/App
    LWIP/Target
    LWIP
    )
set (LWIP_SOURCES
    LWIP/App/lwip.c
    LWIP/Target/ethernetif.c
    LWIP/ping.c
    )
add_library(LWIP ${LWIP_SOURCES} )

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

file (GLOB Third_Party_SRC
    Middlewares/Third_Party/LwIP/src/api/*
    Middlewares/Third_Party/LwIP/src/apps/mqtt/mqtt.c
    Middlewares/Third_Party/LwIP/src/core/*
    Middlewares/Third_Party/LwIP/src/core/ipv4/*
    Middlewares/Third_Party/LwIP/src/netif/*
    Middlewares/Third_Party/LwIP/src/netif/ppp/*
    )

add_library(Middlewares
    ${Third_Party_SRC}
    )

add_executable(${PROJECT_NAME}.elf
    ${SOURCES_C}
     ${SOURCES_CPP}
    )

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
    CORE
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
#    GPIO_SC2_LIB
#    AUDIO_LIB
#    RTP_LIB
#    UDP_JSON_LIB
#    Call_Control_LIB
#    Call_Control_SC2_LIB
#    flash_LIB
#    FatFs_LIB
#    Tests_LIB
#    uid_LIB
#    WDT_LIB
    )

set(HEX_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
    COMMENT "Building ${HEX_FILE} ")

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
    COMMENT "Building ${BIN_FILE} ")


