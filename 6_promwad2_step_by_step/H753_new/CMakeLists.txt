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
add_definitions(-DUSE_HAL_DRIVER)
add_definitions(-DARM_MATH_CM7)
add_definitions(-DHAVE_CONFIG_H)
add_definitions(-D__FPU_PRESENT)

#Логгирование
add_definitions(-DLogsEnableRS232Puts)
#add_definitions(-DLogsEnableToConsole)
#add_definitions(-DPrintTaskLogs) #Запускает поток с логами задач

#Выбор типа устройства
#add_definitions(-DSC4) #(Для выбора УПВ SC2 закомментировать эту строку)

#Задание числа клавиш SC4 ( Оставить одну строку )
add_definitions(-DSC4_CFG_0) # 16
#add_definitions(-DSC4_CFG_1) # 32
#add_definitions(-DSC4_CFG_2) # 48
#add_definitions(-DSC4_CFG_3) # 16N
#add_definitions(-DSC4_CFG_4) # 32N

set (RS232  ${CMAKE_HOME_DIRECTORY}/Core/Src/RS232)
set (Debug  ${CMAKE_HOME_DIRECTORY}/Core/Src/Debug)
set (littlefs ${CMAKE_HOME_DIRECTORY}/Middlewares/Third_Party/littlefs)
set (EEPROM  ${CMAKE_HOME_DIRECTORY}/Core/Src/EEPROM)
set (CircularBuffer ${CMAKE_HOME_DIRECTORY}/Middlewares/Third_Party/CircularBuffer)
set (SRAM ${CMAKE_HOME_DIRECTORY}/Core/Src/SRAM)
set (JSON  ${CMAKE_HOME_DIRECTORY}/Core/Src/JSON)
set (ArduinoJson ${CMAKE_HOME_DIRECTORY}/Middlewares/Third_Party/ArduinoJson)
set (RTC  ${CMAKE_HOME_DIRECTORY}/Core/Src/RTC)
set (SNTP  ${CMAKE_HOME_DIRECTORY}/Core/Src/SNTP)
set (TRNG  ${CMAKE_HOME_DIRECTORY}/Core/Src/TRNG)
set (GPIO  ${CMAKE_HOME_DIRECTORY}/Core/Src/GPIO)
set (UpdateMechanism  ${CMAKE_HOME_DIRECTORY}/Core/Src/UpdateMechanism)
set (TraceRecorder  ${CMAKE_HOME_DIRECTORY}/Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder)
set (RTP  ${CMAKE_HOME_DIRECTORY}/Core/Src/RTP)
set (AUDIO  ${CMAKE_HOME_DIRECTORY}/Core/Src/AUDIO)
set (UDP_JSON  ${CMAKE_HOME_DIRECTORY}/Core/Src/UDP_JSON)
set (Call_Control  ${CMAKE_HOME_DIRECTORY}/Core/Src/Call_Control)
set (flash  ${CMAKE_HOME_DIRECTORY}/Core/Src/flash)
set (FatFs ${CMAKE_HOME_DIRECTORY}/Middlewares/Third_Party/FatFs/src)
set (Tests  ${CMAKE_HOME_DIRECTORY}/Core/Src/Tests)
set (uid  ${CMAKE_HOME_DIRECTORY}/Core/Src/uid)
set (WDT  ${CMAKE_HOME_DIRECTORY}/Core/Src/WDT)
set (STARTUP_SOURCES ${CMAKE_HOME_DIRECTORY}/Core/startup/startup_stm32h753iitx.s)
set (SEGGER_RTT  ${CMAKE_HOME_DIRECTORY}/SEGGER_RTT_V767d)

include_directories(
    Core/Inc
    Drivers/CMSIS/Include
    Drivers/CMSIS/DSP/Include
    Drivers/CMSIS/Device/ST/STM32H7xx/Include
    Drivers/BSP/Components/dp83848
    Drivers/BSP/Components/dp83848
    Drivers/STM32H7xx_HAL_Driver/Inc
    Drivers/STM32H7xx_HAL_Driver/Inc/Legacy
    Middlewares/Third_Party/FreeRTOS/Source/include
    Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS
    Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang
    Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
    Middlewares/Third_Party/ArduinoJson/src
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
    ${RS232}
    ${Debug}
    ${littlefs}
    ${EEPROM}
    ${CircularBuffer}
    ${SRAM}
    ${JSON}
    ${ArduinoJson}
    ${RTC}
    ${SNTP}
    ${TRNG}
    ${GPIO}
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
    Core/Src/LWIP/App
    Core/Src/LWIP/Target
    ${SEGGER_RTT}/RTT
    ${SEGGER_RTT}/Config
    )

file (GLOB SOURCES_C Core/Src/*.c STM32H753IITX_FLASH.ld Drivers/BSP/Components/dp83848/dp83848.c )
list(REMOVE_ITEM SOURCES_C ${CMAKE_HOME_DIRECTORY}/Core/Src/net_sockets.c )
file (GLOB SOURCES_CPP Core/Src/*.cpp)
file (GLOB RS232_src ${RS232}/*.cpp ${RS232}/*.c ${RS232}/*.h )
file (GLOB Debug_src ${Debug}/*.cpp ${Debug}/*.c ${Debug}/*.h )
file (GLOB EEPROM_src ${EEPROM}/*.cpp ${EEPROM}/*.c ${EEPROM}/*.h )
file (GLOB littlefs_src ${littlefs}/*.c ${littlefs}/*.h  )
file (GLOB CircularBuffer_src ${CircularBuffer}/*.tpp ${CircularBuffer}/*.hpp ${CircularBuffer}/*.h )
file (GLOB SRAM_src ${SRAM}/*.cpp ${SRAM}/*.h  Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sram.c)
file (GLOB JSON_src ${JSON}/*.cpp ${JSON}/*.h )
file (GLOB RTC_src ${RTC}/*.cpp ${RTC}/*.h )
file (GLOB SNTP_src ${SNTP}/*.cpp ${SNTP}/*.h )
file (GLOB TRNG_src ${TRNG}/*.cpp ${TRNG}/*.h )
file (GLOB GPIO_SOURCES ${GPIO}/*.cpp ${GPIO}/*.h)
file (GLOB UpdateMechanism_src ${UpdateMechanism}/*.cpp ${UpdateMechanism}/*.h )
file (GLOB TraceRecorder_src ${TraceRecorder}/*.c ${TraceRecorder}/streamports/TCPIP/*.c ${TraceRecorder}/streamports/TCPIP/include/*.h ${TraceRecorder}/include/*.h ${TraceRecorder}/config/*.h )
file (GLOB RTP_src ${RTP}/*.cpp ${RTP}/*.h )
file (GLOB AUDIO_src ${AUDIO}/*.cpp ${AUDIO}/*.h )
file (GLOB UDP_JSON_src ${UDP_JSON}/*.cpp ${UDP_JSON}/*.h )
file (GLOB Call_Control_src ${Call_Control}/*.cpp ${Call_Control}/*.h )
list(REMOVE_ITEM Call_Control_src ${Call_Control}/call_control.h ${Call_Control}/call_control.cpp )

file (GLOB flash_src ${flash}/*.cpp ${flash}/*.h )
file (GLOB FatFs_src ${FatFs}/*.c ${FatFs}/option/syscall.c ${FatFs}/option/unicode.c ${FatFs}/*.h )
file (GLOB Tests_src ${Tests}/*.cpp ${Tests}/*.h )
file (GLOB uid_src ${uid}/*.cpp ${uid}/*.h )
file (GLOB WDT_src ${WDT}/*.cpp ${WDT}/*.h )
file (GLOB HAL_SRC Drivers/STM32H7xx_HAL_Driver/Src/*)
list(REMOVE_ITEM HAL_SRC ${CMAKE_HOME_DIRECTORY}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rtc.c)

file (GLOB CMSIS_RTOS_SOURCES Middlewares/Third_Party/FreeRTOS/Source/*.c Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c Middlewares/Third_Party/LwIP/src/core/*.c Middlewares/Third_Party/LwIP/system/OS/*.c  Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/*.c )
file (GLOB LWIP_SRC Middlewares/Third_Party/LwIP/src/api/* Middlewares/Third_Party/LwIP/src/apps/mqtt/mqtt.c Middlewares/Third_Party/LwIP/src/core/* Middlewares/Third_Party/LwIP/src/core/ipv4/* Middlewares/Third_Party/LwIP/src/netif/* Middlewares/Third_Party/LwIP/src/netif/ppp/* )
file (GLOB SEGGER_RTT_SOURCES ${SEGGER_RTT}/RTT/*.c ${SEGGER_RTT}/RTT/SEGGER_RTT_ASM_ARMv7M.S ${SEGGER_RTT}/Syscalls/*.c)

add_library(RS232_LIB ${RS232_src} )
add_library(Debug_LIB ${Debug_src} )
add_library(EEPROM_LIB ${EEPROM_src} )
add_library(littlefs_LIB ${littlefs_src} )
add_library(CircularBuffer_LIB ${CircularBuffer_src} )
set_target_properties(CircularBuffer_LIB PROPERTIES LINKER_LANGUAGE CXX)
add_library(SRAM_LIB ${SRAM_src} )
add_library(JSON_LIB ${JSON_src} )
add_library(RTC_LIB ${RTC_src} )
add_library(SNTP_LIB ${SNTP_src} )
add_library(TRNG_LIB ${TRNG_src} )
add_library(GPIO_LIB ${GPIO_SOURCES} )
add_library(UpdateMechanism_LIB ${UpdateMechanism_src} )
add_library(TraceRecorder_LIB ${TraceRecorder_src} )
add_library(RTP_LIB ${RTP_src} )
add_library(AUDIO_LIB ${AUDIO_src})
add_library(UDP_JSON_LIB ${UDP_JSON_src} )
add_library(Call_Control_LIB ${Call_Control_src} )
add_library(flash_LIB ${flash_src} )
add_library(FatFs_LIB ${FatFs_src} )
add_library(Tests_LIB ${Tests_src} )
add_library(uid_LIB ${uid_src} )
add_library(WDT_LIB ${WDT_src} )
add_library(STARTUP ${STARTUP_SOURCES} )
add_library(STM32H7xx_HAL_Driver ${HAL_SRC})
add_library(CMSIS_RTOS ${CMSIS_RTOS_SOURCES} )
add_library(LWIP ${LWIP_SRC} )
add_library(RTT ${SEGGER_RTT_SOURCES})

add_executable(${PROJECT_NAME}.elf
    ${SOURCES_C}
    ${SOURCES_CPP}
    Core/Src/LWIP/App/lwip.c
    Core/Src/LWIP/Target/ethernetif.c
    Middlewares/Third_Party/LwIP/src/api/sockets.c
    Core/Src/RTP/rtp.cpp
    Drivers/CMSIS/DSP/Source/SupportFunctions/arm_copy_q7.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sai.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_wwdg.c
    Drivers/CMSIS/DSP/Source/SupportFunctions/arm_copy_q15.c
    Drivers/CMSIS/DSP/Source/SupportFunctions/arm_fill_q15.c
    Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_add_q15.c
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
    -u_printf_float \
    -fno-exceptions \
    -Wl,--end-group \
    ")
#    -Wl,-section-start=.rtt=0x20000000 \
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
    -u_printf_float \
    -fno-exceptions \
    -Wl,--end-group \
    ")
#-Wl,-section-start=.rtt=0x20000000 \
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
#set(LINKER_SCRIPT_RAM "${CMAKE_SOURCE_DIR}/STM32H753IITX_RAM.ld")
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
    SRAM_LIB
    STM32H7xx_HAL_Driver
    LWIP
    CMSIS_RTOS
    RS232_LIB
    Debug_LIB
    EEPROM_LIB
    littlefs_LIB
    CircularBuffer_LIB
    JSON_LIB
    RTC_LIB
    SNTP_LIB
    TRNG_LIB
    GPIO_LIB
    UpdateMechanism_LIB
    TraceRecorder_LIB
    RTP_LIB
    UDP_JSON_LIB
    Call_Control_LIB
    FatFs_LIB
    Tests_LIB
    uid_LIB
    WDT_LIB
    AUDIO_LIB
    flash_LIB
    RTT
    )

set(HEX_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin)
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
    COMMENT "Building ${HEX_FILE} ")
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
    COMMENT "Building ${BIN_FILE} ")
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD  COMMAND arm-none-eabi-size ${PROJECT_NAME}.elf )

