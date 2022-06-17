cmake_minimum_required(VERSION 3.15)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Set ARM compiler
#set(TOOLS_PATH /home/evg/toolchain/gcc-arm-none-eabi-new)
#set(CMAKE_C_COMPILER ${TOOLS_PATH}/bin/arm-none-eabi-gcc)
#set(CMAKE_CXX_COMPILER ${TOOLS_PATH}/bin/arm-none-eabi-g++)
#set(CMAKE_CXX_LINKER_LAUNCHER ${TOOLS_PATH}/bin/arm-none-eabi-g++)
#set(CMAKE_OBJCOPY ${TOOLS_PATH}/bin/arm-none-eabi-objcopy)

project(H753_new C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_definitions(-DSTM32H753xx)
add_definitions(-DDATA_IN_D2_SRAM)
add_definitions(-DUSE_HAL_DRIVER)
add_definitions(-DSC2BOARD)

set (RS232  Core/Src/RS232)
set (Debug  Core/Src/Debug)
set (littlefs Middlewares/Third_Party/littlefs)
set (EEPROM  Core/Src/EEPROM)
set (CircularBuffer Middlewares/Third_Party/CircularBuffer)
set (SRAM Core/Src/SRAM)
set (JSON  Core/Src/JSON)
set (ArduinoJson Middlewares/Third_Party/ArduinoJson)
set (RTC  Core/Src/RTC)
set (SNTP  Core/Src/SNTP)
set (TRNG  Core/Src/TRNG)
set (GPIO_SC2  Core/Src/GPIO_SC2_BOARD)
set (UpdateMechanism  Core/Src/UpdateMechanism)
set (TraceRecorder  "Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder")
set (RTP  Core/Src/RTP)
set (AUDIO  Core/Src/AUDIO)
set (UDP_JSON  Core/Src/UDP_JSON)
set (Call_Control  Core/Src/Call_Control)
set (flash  Core/Src/flash)
set (FatFs Middlewares/Third_Party/FatFs/src)
set (Tests  Core/Src/Tests)
set (uid  Core/Src/uid)
set (WDT  Core/Src/WDT)
set (STARTUP_SOURCES Core/startup/startup_stm32h753iitx.s)

include_directories(
    Core/Inc
    Drivers/CMSIS/Include
    Drivers/CMSIS/DSP/Include
    Drivers/CMSIS/Device/ST/STM32H7xx/Include
    Drivers/BSP/Components/dp83848
    Drivers/BSP/Components/dp83848
    ${RS232}
    ${Debug}
    ${littlefs}
    ${EEPROM}
    ${CircularBuffer}
    ${SRAM}
    ${JSON}
    ${ArduinoJson}
    Middlewares/Third_Party/ArduinoJson/src
    ${RTC}
    ${SNTP}
    ${TRNG}
    ${GPIO_SC2}
    ${UpdateMechanism}
    ${TraceRecorder}/streamports/TCPIP/include
    ${TraceRecorder}/include
    ${TraceRecorder}/config
    ${RTP}
    ${AUDIO}
    ${UDP_JSON}
    ${Call_Control}
    ${flash}
    ${FatFs}
    ${Tests}
    ${uid}
    ${WDT}


    )

file (GLOB SOURCES_C Core/Src/*.c STM32H753IITX_FLASH.ld Drivers/BSP/Components/dp83848/dp83848.c )
file (GLOB SOURCES_CPP Core/Src/*.cpp)
file (GLOB RS232_src ${RS232}/*.cpp ${RS232}/*.c ${RS232}/*.h )
add_library(RS232_LIB ${RS232_src} )

file (GLOB Debug_src ${Debug}/*.cpp ${Debug}/*.c ${Debug}/*.h )
add_library(Debug_LIB ${Debug_src} )

file (GLOB EEPROM_src ${EEPROM}/*.cpp ${EEPROM}/*.c ${EEPROM}/*.h )
add_library(EEPROM_LIB ${EEPROM_src} )

file (GLOB littlefs_src ${littlefs}/*.c ${littlefs}/*.h  )
add_library(littlefs_LIB ${littlefs_src} )

file (GLOB CircularBuffer_src ${CircularBuffer}/*.tpp ${CircularBuffer}/*.hpp ${CircularBuffer}/*.h )
add_library(CircularBuffer_LIB ${CircularBuffer_src} )
set_target_properties(CircularBuffer_LIB PROPERTIES LINKER_LANGUAGE CXX)

file (GLOB SRAM_src ${SRAM}/*.cpp ${SRAM}/*.h  Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sram.c)
add_library(SRAM_LIB ${SRAM_src} )

file (GLOB JSON_src ${JSON}/*.cpp ${JSON}/*.h )
add_library(JSON_LIB ${JSON_src} )

file (GLOB RTC_src ${RTC}/*.cpp ${RTC}/*.h )
add_library(RTC_LIB ${RTC_src} )

file (GLOB SNTP_src ${SNTP}/*.cpp ${SNTP}/*.h )
add_library(SNTP_LIB ${SNTP_src} )

file (GLOB TRNG_src ${TRNG}/*.cpp ${TRNG}/*.h )
add_library(TRNG_LIB ${TRNG_src} )

file (GLOB GPIO_SC2_SOURCES ${GPIO_SC2}/*.cpp ${GPIO_SC2}/*.h)
add_library(GPIO_SC2_LIB ${GPIO_SC2_SOURCES} )

file (GLOB UpdateMechanism_src ${UpdateMechanism}/*.cpp ${UpdateMechanism}/*.h )
add_library(UpdateMechanism_LIB ${UpdateMechanism_src} )

file (GLOB TraceRecorder_src ${TraceRecorder}/*.c ${TraceRecorder}/streamports/TCPIP/*.c ${TraceRecorder}/streamports/TCPIP/include/*.h ${TraceRecorder}/include/*.h ${TraceRecorder}/config/*.h )
add_library(TraceRecorder_LIB ${TraceRecorder_src} )

#file (GLOB RTP_src ${RTP}/*.cpp ${RTP}/*.h )
#add_library(RTP_LIB ${RTP_src} )

#file (GLOB AUDIO_src ${AUDIO}/*.cpp ${AUDIO}/*.h )
#add_library(AUDIO_LIB ${AUDIO_src} )

#file (GLOB UDP_JSON_src ${UDP_JSON}/*.cpp ${UDP_JSON}/*.h )
#add_library(UDP_JSON_LIB ${UDP_JSON_src} )

#file (GLOB Call_Control_src ${Call_Control}/*.cpp ${Call_Control}/*.h )
#list(REMOVE_ITEM Call_Control_src ${Call_Control}/call_control.h ${Call_Control}/call_control.cpp )
#add_library(Call_Control_LIB ${Call_Control_src} )

file (GLOB flash_src ${flash}/*.cpp ${flash}/*.h )
add_library(flash_LIB ${flash_src} )

file (GLOB FatFs_src ${FatFs}/*.c ${FatFs}/option/syscall.c ${FatFs}/option/unicode.c ${FatFs}/*.h )
add_library(FatFs_LIB ${FatFs_src} )

file (GLOB Tests_src ${Tests}/*.cpp ${Tests}/*.h )
add_library(Tests_LIB ${Tests_src} )

file (GLOB uid_src ${uid}/*.cpp ${uid}/*.h )
add_library(uid_LIB ${uid_src} )

file (GLOB WDT_src ${WDT}/*.cpp ${WDT}/*.h )
add_library(WDT_LIB ${WDT_src} )

add_library(STARTUP ${STARTUP_SOURCES} )

#set (BSP_SOURCES Drivers/BSP/Components/lan8742/lan8742.c )
#include_directories( Drivers/BSP/Components/lan8742 )
#add_library(BSP ${BSP_SOURCES} )

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

include_directories(Core/Src/LWIP/App Core/Src/LWIP/Target)
add_executable(${PROJECT_NAME}.elf
    ${SOURCES_C}
    ${SOURCES_CPP}
    Core/Src/LWIP/App/lwip.c
    Core/Src/LWIP/Target/ethernetif.c
    )

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
    STARTUP
#    BSP
    SRAM_LIB
    flash_LIB
    STM32H7xx_HAL_Driver
    Middlewares
    CMSIS_RTOS
    RS232_LIB
    Debug_LIB
    EEPROM_LIB
    littlefs_LIB
    CircularBuffer_LIB
    JSON_LIB
#    RTC_LIB
#    SNTP_LIB
#    TRNG_LIB
    GPIO_SC2_LIB
#    UpdateMechanism_LIB
#    TraceRecorder_LIB
#    RTP_LIB
#    AUDIO_LIB
#    UDP_JSON_LIB
#    Call_Control_LIB
    FatFs_LIB
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


