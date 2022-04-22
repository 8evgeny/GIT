cmake_minimum_required(VERSION 3.15)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Set ARM compiler
#set(TOOLS_PATH /home/evg/toolchain/gcc-arm-none-eabi-new)
#set(TOOLS_PATH /home/evg/toolchain/gcc-arm-none-eabi-4_9-2014q4)
#set(CMAKE_C_COMPILER ${TOOLS_PATH}/bin/arm-none-eabi-gcc)
#set(CMAKE_CXX_COMPILER ${TOOLS_PATH}/bin/arm-none-eabi-g++)
#set(CMAKE_CXX_LINKER_LAUNCHER ${TOOLS_PATH}/bin/arm-none-eabi-g++)
#set(CMAKE_OBJCOPY ${TOOLS_PATH}/bin/arm-none-eabi-objcopy)

project(RTOS_753 C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set (Third_Party "Middlewares/Third_Party")
set (SRC "Src/")

    file (GLOB SRC_src
    ${SRC}*.c
    ${SRC}main.cpp
    ${SRC}flash_diskio.cpp
    ${SRC}../syscalls.c
    )

    set (FatFs ${Third_Party}/FatFs)
    file (GLOB FatFs_src
        ${FatFs}/src/*.c ${FatFs}/src/option/syscall.c ${FatFs}/src/option/unicode.c
        ${FatFs}/src/*.h
        )
#    include_directories(${FatFs}/src )


    set (UpdateMechanism  ${SRC}UpdateMechanism)
    file (GLOB UpdateMechanism_src ${UpdateMechanism}/*.cpp
        ${UpdateMechanism}/*.h

        )
#    include_directories(${UpdateMechanism})

    set (EEPROM  ${SRC}EEPROM)
    file (GLOB_RECURSE EEPROM_src ${EEPROM}/*.cpp ${EEPROM}/*.c
        ${EEPROM}/*.h
        )
#    include_directories(${EEPROM})

    set (TraceRecorder  "Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder")
    file (GLOB TraceRecorder_src ${TraceRecorder}/*.c ${TraceRecorder}/streamports/TCPIP/*.c
        ${TraceRecorder}/streamports/TCPIP/include/*.h ${TraceRecorder}/include/*.h ${TraceRecorder}/config/*.h
        )
#    include_directories(${TraceRecorder}/streamports/TCPIP/include ${TraceRecorder}/include ${TraceRecorder}/config )

    set (Tests  ${SRC}Tests)
    file (GLOB Tests_src ${Tests}/*.cpp
        ${Tests}/*.h
        )
#    include_directories(${Tests})

    set (UID  ${SRC}uid)
    file (GLOB UID_src ${UID}/*.cpp
        ${UID}/*.h
        )
#    include_directories(${UID})

#Проверить RTT
    set (RTT  ${CMAKE_HOME_DIRECTORY}/RTT)
    file (GLOB RTT_src ${RTT}/*.c
        ${RTT}/*.h
        )
#    include_directories( ${RTT} )

    set (RTP  ${SRC}RTP)
    file (GLOB RTP_src ${RTP}/*.cpp
        ${RTP}/*.h
        )
#    include_directories( ${RTP} )

set (WDT  ${SRC}WDT)
file (GLOB WDT_src ${WDT}/*.cpp
    ${WDT}/*.h
    )
#include_directories( ${WDT} )


    set (AUDIO  ${SRC}AUDIO)
    file (GLOB AUDIO_src ${AUDIO}/*.cpp
        ${AUDIO}/*.h
        )
#    include_directories( ${AUDIO} )

    set (Flash  ${SRC}flash)
    file (GLOB Flash_src ${Flash}/*.cpp
        ${Flash}/*.h
        )
#    include_directories( ${Flash} )

    set (TRNG  ${SRC}TRNG)
    file (GLOB TRNG_src ${TRNG}/*.cpp
        ${TRNG}/*.h
        )
#    include_directories( ${TRNG} )

set (Debug  ${SRC}Debug)
file (GLOB Debug_src ${Debug}/*.cpp ${Debug}/*.c
    ${Debug}/*.h
    )
#include_directories( ${Debug} )

    set (RS232  ${SRC}RS232)
    file (GLOB RS232_src ${RS232}/*.cpp ${RS232}/*.c
        ${RS232}/*.h
        )
#    include_directories( ${RS232} )


    set (GPIO_SC2_BOARD  ${SRC}GPIO_SC2_BOARD)
    file (GLOB GPIO_SC2_BOARD_src ${GPIO_SC2_BOARD}/*.cpp
        ${GPIO_SC2_BOARD}/*.h
        )
#    include_directories( ${GPIO_SC2_BOARD} )

    set (RTC  ${SRC}RTC)
    file (GLOB RTC_src ${RTC}/*.cpp
        ${RTC}/*.h
        )
#    include_directories( ${RTC} )

    set (SRAM  ${SRC}SRAM)
    file (GLOB SRAM_src ${SRAM}/*.cpp
        ${SRAM}/*.h
        )
#    include_directories( ${SRAM} )

    set (JSON  ${SRC}JSON)
    file (GLOB JSON_src ${JSON}/*.cpp
        ${JSON}/*.h
        )
#    include_directories( ${JSON} )

    set (UDP_JSON  ${SRC}UDP_JSON)
    file (GLOB UDP_JSON_src ${UDP_JSON}/*.cpp
        ${UDP_JSON}/*.h
        )
#    include_directories( ${UDP_JSON} )

set (LwIP ${Third_Party}/LwIP)
    file (GLOB LwIP_src
        ${LwIP}/src/netif/*.c
        ${LwIP}/src/api/*.c
        ${LwIP}/src/apps/sntp/*.c
        ${LwIP}/src/core/*.c
        ${LwIP}/src/api/*.c
        ${LwIP}/src/core/ipv4/*.c
        ${LwIP}/system/OS/*.c
        ${LwIP}/src/include/*/*.h
        ${LwIP}/src/include/*/*/*.h
        ${LwIP}/system/*.h
        ${LwIP}/system/OS/*.h
        )
#    include_directories(
#        ${LwIP}/src/include/*/*.h
#        ${LwIP}/src/include/*/*/*.h
#        ${LwIP}/system/*.h
#        ${LwIP}/system/OS/*.h
#        ${LwIP}/system/*.h
#        ${LwIP}/src/include
#        ${LwIP}/system
#        ${LwIP}/system/OS
#        ${LwIP}/src
#        )

    set (littlefs ${Third_Party}/littlefs)
    file (GLOB littlefs_src ${littlefs}/*.c
        ${littlefs}/*.h
        )
#    include_directories( ${littlefs} )

    set (ArduinoJson ${Third_Party}/ArduinoJson)
    file (GLOB ArduinoJson_src ${ArduinoJson}/*.h )
#    include_directories( ${ArduinoJson} ${ArduinoJson}/src ${ArduinoJson}/src/ArduinoJson)

set (CircularBuffer ${Third_Party}/CircularBuffer)
file (GLOB CircularBuffer_src ${CircularBuffer}/*.c
    ${CircularBuffer}/*.h
    )
#include_directories( ${CircularBuffer} )

    set (FreeRTOS ${Third_Party}/FreeRTOS)
    file (GLOB FreeRTOS_src2 ${FreeRTOS}/Source/portable/MemMang/heap_4.c )

set (CMSIS_RTOS ${Third_Party}/FreeRTOS/CMSIS_RTOS)
file (GLOB CMSIS_RTOS_src ${CMSIS_RTOS}/*.c
    ${CMSIS_RTOS}/*.h
    )
#include_directories( ${CMSIS_RTOS} )

    set (Call_Control  ${SRC}Call_Control)
    file (GLOB Call_Control_src ${Call_Control}/*.cpp
        ${Call_Control}/*.h
        )
#    list(REMOVE_ITEM Call_Control_src
#        ${Call_Control}/call_control.h
#        ${Call_Control}/call_control.cpp
#        )
#    include_directories( ${Call_Control} )

    set (SNTP  ${SRC}SNTP)
    file (GLOB SNTP_src ${SNTP}/*.cpp
        ${SNTP}/*.h
        )
#    include_directories( ${SNTP} )

set (INC  ${SRC}Inc)
file (GLOB INC_src ${INC}/*.h)
#    include_directories( Inc )

    set (startup  ${SRC}startup)
    file (GLOB startup_src ${startup}/startup_stm32f777xx.s )
#    file (GLOB startup_src ${startup}/startup_stm32h753iitx.s )

file (GLOB LD_src STM32F777IITx_FLASH.ld )
#file (GLOB LD_src newSTM32H753IITX_FLASH.ld )

file (GLOB HZ_src ${CMAKE_HOME_DIRECTORY}/Drivers/CMSIS_STM32F7xx/DSP/Source/*.c )
#file (GLOB HZ_src2 ${CMAKE_HOME_DIRECTORY}/Drivers/STM32F7xx_HAL_Driver/Src/*.c )
file (GLOB HZ_src3 ${CMAKE_HOME_DIRECTORY}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/*.c )
file (GLOB HZ_src4 ${CMAKE_HOME_DIRECTORY}/Drivers/STM32F7xx_HAL_Driver/*.c )
file (GLOB HZ_src5 ${CMAKE_HOME_DIRECTORY}/Middlewares/Third_Party/FreeRTOS/Source/*.c)

    include_directories(
    ${CMAKE_HOME_DIRECTORY}
    ${CMSIS_STM32F7xx}
    ${CMSIS_STM32F7xx}/Include
    ${CMSIS_STM32F7xx}/Device/ST/STM32F7xx/Include
    ${CMSIS_STM32F7xx}/DSP/Include,
    Inc
    ${FreeRTOS}/Source/include
    ${FreeRTOS}/Source/portable/GCC/ARM_CM7/r0p1
    ${CMSIS_RTOS}
    ${STM32F7xx_HAL}/Inc
    ${STM32F7xx_HAL}/Inc/Legacy
    ${TraceRecorder}/streamports/TCPIP/include
    ${TraceRecorder}/include
    ${TraceRecorder}/config
    ${LwIP}/src/include
    ${LwIP}/system
    ${LwIP}/system/OS
    ${LwIP}/src
    ${ArduinoJson}
    ${ArduinoJson}/src
    ${ArduinoJson}/src/ArduinoJson
    ${CircularBuffer}
    ${littlefs}
    ${FatFs}/src
    ${RTP}
    ${Audio}
    ${UID}
    ${Flash}
    ${EEPROM}
    ${GPIO_SC2_BOARD}
    ${RTT}
    ${RTC}
    ${SRAM}
    ${RS232}
    ${JSON}
    ${WDT}
    ${UDP_JSON}
    ${Call_Control}

    )

include_directories(
    ${FreeRTOS}/Source/CMSIS_RTOS
    Drivers/CMSIS_STM32F7xx/DSP/Include
    Drivers/CMSIS_STM32F7xx/Core/Include
    Drivers/STM32F7xx_HAL_Driver/Inc
    Drivers/CMSIS_STM32F7xx/Device/ST/STM32F7xx/Include
    Src/CAN_STM32F7xx
)


add_executable(${PROJECT_NAME}.elf
    ${FatFs_src}
    ${Bootloader_src}
    ${UpdateMechanism_src}
    ${EEPROM_src}
    ${TraceRecorder_src}
    ${Tests_src}
    ${UID_src}
    ${RTT_src}
    ${RTP_src}
    ${WDT_src}
    ${RTC_src}
    ${AUDIO_src}
    ${Flash_src}
    ${TRNG_src}
    ${Debug_src}
    ${RS232_src}
    ${CAN_STM32F7xx_src}
    ${GPIO_STM32F7xx_src}
    ${GPIO_SC2_BOARD_src}
    ${RTC_src}
    ${SRAM_src}
    ${JSON_src}
    ${UDP_JSON_src}
    ${LwIP_src}
    ${littlefs_src}
    ${ArduinoJson_src}
    ${CircularBuffer_src}
    ${FreeRTOS_src}
    ${FreeRTOS_src2}
    ${CMSIS_RTOS_src}
    ${STM32F7xx_HAL_src}
    ${CMSIS_STM32F7xx_src}
    ${Call_Control_src}
    ${SNTP_src}
    ${INC_src}
    ${SRC_src}
    ${startup_src}
    ${LD_src}
    Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c
    Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.h
    Src/Call_Control/os_timers.cpp
    Middlewares/Third_Party/FreeRTOS/Source/timers.c
    ${HZ_src}
    ${HZ_src2}
    ${HZ_src3}
    ${HZ_src4}
    ${HZ_src5}
    Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_can.c
    Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.c
    Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma.c
    Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.c
    Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_eth.c
    Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sai.c
    Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c_ex.c
    Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.c
    Drivers/CMSIS_STM32F7xx/DSP/Source/SupportFunctions/arm_copy_q7.c
    Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/port.c
    Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c.c

    )

#add_definitions(-DSTM32F753xx)



    add_definitions(-DSTM32F777xx)
    add_definitions(-DUSE_HAL_DRIVER)
#    add_definitions(-D__weak="__attribute__((weak))")
#    add_definitions(-D__packed="__attribute__((__packed__))")
    add_definitions(-D__FPU_PRESENT=1)
    add_definitions(-DARM_MATH_CM7)
    add_definitions(-DHAVE_CONFIG_H)
    add_definitions(-D__DCACHE_PRESENT)
    add_definitions(-D__ICACHE_PRESENT)
    add_definitions(-DSC2BOARD)

#CompilerFlags

#    -specs=nosys.specs \
#    -specs=nano.specs \

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
#    -fno-rtti \
#    -std=gnu11 \
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

    set(LINKER_SCRIPT_FLASH1 "${CMAKE_SOURCE_DIR}/STM32F777IITx_FLASH.ld")
    #set(LINKER_SCRIPT_FLASH "${CMAKE_SOURCE_DIR}/newSTM32H753IITX_FLASH.ld")

    set(LINKER_FLAGS "SHELL: \
        -T${LINKER_SCRIPT_FLASH1} \
        -Wl,--start-group \
        -Wl,--gc-sections \
        -mcpu=cortex-m7 \
        --specs=nosys.specs \
        -Wl,--gc-sections \
        -static \
        --specs=nano.specs \
        -mfpu=fpv5-d16 \
        -mfloat-abi=hard \
        -mthumb
        -lgcc \
        -lc \
        -lstdc++ \
        -lm \
        -lnosys \
        -Wl,--end-group \
        ")

target_link_options(${PROJECT_NAME}.elf PRIVATE ${LINKER_FLAGS})

#set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINKER_LANGUAGE C)
set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINKER_LANGUAGE CXX)

set(HEX_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
    COMMENT "Building ${HEX_FILE} ")

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
    COMMENT "Building ${BIN_FILE} ")


