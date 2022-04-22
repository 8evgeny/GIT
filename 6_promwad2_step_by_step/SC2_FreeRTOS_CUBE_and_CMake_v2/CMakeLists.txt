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

    file (GLOB MAIN_src Src/*.c Src/main.cpp Src/flash_diskio.cpp Src/../syscalls.c )

    set (FatFs Middlewares/Third_Party/FatFs)
    file (GLOB FatFs_src ${FatFs}/src/*.c ${FatFs}/src/option/syscall.c ${FatFs}/src/option/unicode.c ${FatFs}/src/*.h )

    set (UpdateMechanism  Src/UpdateMechanism)
    file (GLOB UpdateMechanism_src ${UpdateMechanism}/*.cpp ${UpdateMechanism}/*.h )

    set (EEPROM  Src/EEPROM)
    file (GLOB_RECURSE EEPROM_src ${EEPROM}/*.cpp ${EEPROM}/*.c ${EEPROM}/*.h )

    set (TraceRecorder  "Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder")
    file (GLOB TraceRecorder_src ${TraceRecorder}/*.c ${TraceRecorder}/streamports/TCPIP/*.c ${TraceRecorder}/streamports/TCPIP/include/*.h ${TraceRecorder}/include/*.h ${TraceRecorder}/config/*.h )

    set (Tests  Src/Tests)
    file (GLOB Tests_src ${Tests}/*.cpp ${Tests}/*.h )

    set (UID  Src/uid)
    file (GLOB UID_src ${UID}/*.cpp ${UID}/*.h )

    set (RTT  ${CMAKE_HOME_DIRECTORY}/RTT)
    file (GLOB RTT_src ${RTT}/*.c ${RTT}/*.h )

    set (RTP  Src/RTP)
    file (GLOB RTP_src ${RTP}/*.cpp ${RTP}/*.h )

    set (WDT  Src/WDT)
    file (GLOB WDT_src ${WDT}/*.cpp ${WDT}/*.h )

    set (AUDIO  Src/AUDIO)
    file (GLOB AUDIO_src ${AUDIO}/*.cpp ${AUDIO}/*.h )

    set (Flash  Src/flash)
    file (GLOB Flash_src ${Flash}/*.cpp ${Flash}/*.h )

    set (TRNG  Src/TRNG)
    file (GLOB TRNG_src ${TRNG}/*.cpp ${TRNG}/*.h )

    set (Debug  Src/Debug)
    file (GLOB Debug_src ${Debug}/*.cpp ${Debug}/*.c ${Debug}/*.h )

    set (RS232  Src/RS232)
    file (GLOB RS232_src ${RS232}/*.cpp ${RS232}/*.c ${RS232}/*.h )

    set (GPIO_SC2_BOARD  Src/GPIO_SC2_BOARD)
    file (GLOB GPIO_SC2_BOARD_src ${GPIO_SC2_BOARD}/*.cpp ${GPIO_SC2_BOARD}/*.h )

    set (RTC  Src/RTC)
    file (GLOB RTC_src ${RTC}/*.cpp ${RTC}/*.h )

    set (SRAM  Src/SRAM)
    file (GLOB SRAM_src ${SRAM}/*.cpp ${SRAM}/*.h )

    set (JSON  Src/JSON)
    file (GLOB JSON_src ${JSON}/*.cpp ${JSON}/*.h )

    set (UDP_JSON  Src/UDP_JSON)
    file (GLOB UDP_JSON_src ${UDP_JSON}/*.cpp ${UDP_JSON}/*.h )

    set (LwIP Middlewares/Third_Party/LwIP)
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

    set (littlefs Middlewares/Third_Party/littlefs)
    file (GLOB littlefs_src ${littlefs}/*.c ${littlefs}/*.h )

    set (ArduinoJson Middlewares/Third_Party/ArduinoJson)
    file (GLOB ArduinoJson_src ${ArduinoJson}/*.h )

    set (CircularBuffer Middlewares/Third_Party/CircularBuffer)
    file (GLOB CircularBuffer_src ${CircularBuffer}/*.c ${CircularBuffer}/*.h )

    set (FreeRTOS Middlewares/Third_Party/FreeRTOS)
    file (GLOB FreeRTOS_src2 ${FreeRTOS}/Source/portable/MemMang/heap_4.c )

    set (CMSIS_RTOS Middlewares/Third_Party/FreeRTOS/CMSIS_RTOS)
    file (GLOB CMSIS_RTOS_src ${CMSIS_RTOS}/*.c ${CMSIS_RTOS}/*.h )

    set (Call_Control  Src/Call_Control)
    file (GLOB Call_Control_src ${Call_Control}/*.cpp ${Call_Control}/*.h )
    list(REMOVE_ITEM Call_Control_src ${Call_Control}/call_control.h ${Call_Control}/call_control.cpp )

    set (SNTP  Src/SNTP)
    file (GLOB SNTP_src ${SNTP}/*.cpp ${SNTP}/*.h )

    set (INC  Src/Inc)
    file (GLOB INC_src ${INC}/*.h)

    set (startup  Src/startup)
    file (GLOB startup_src ${startup}/startup_stm32f777xx.s )

    file (GLOB LD_src STM32F777IITx_FLASH.ld )
    #file (GLOB LD_src newSTM32H753IITX_FLASH.ld )

    file (GLOB HZ_src ${CMAKE_HOME_DIRECTORY}/Drivers/CMSIS_STM32h7xx/DSP/Source/*.c )
    #file (GLOB HZ_src2 ${CMAKE_HOME_DIRECTORY}/Drivers/STM32H7xx_HAL_Driver/Src/*.c )
    file (GLOB HZ_src3 ${CMAKE_HOME_DIRECTORY}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/*.c )
    file (GLOB HZ_src4 ${CMAKE_HOME_DIRECTORY}/Drivers/STM32H7xx_HAL_Driver/*.c )
    file (GLOB HZ_src5 ${CMAKE_HOME_DIRECTORY}/Middlewares/Third_Party/FreeRTOS/Source/*.c)

include_directories(
    ${CMAKE_HOME_DIRECTORY}
    ${CMSIS_STM32h7xx}
    ${CMSIS_STM32h7xx}/Include
    ${CMSIS_STM32h7xx}/Device/ST/STM32h7xx/Include
    ${CMSIS_STM32h7xx}/DSP/Include,
    Inc
    ${FreeRTOS}/Source/include
    ${FreeRTOS}/Source/portable/GCC/ARM_CM7/r0p1
    ${CMSIS_RTOS}
    ${STM32h7xx_HAL}/Inc
    ${STM32h7xx_HAL}/Inc/Legacy
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
    ${FreeRTOS}/Source/CMSIS_RTOS
    Drivers/CMSIS_STM32h7xx/DSP/Include
    Drivers/CMSIS_STM32h7xx/Core/Include
    Drivers/STM32H7xx_HAL_Driver/Inc
    Drivers/CMSIS_STM32h7xx/Device/ST/STM32h7xx/Include
    Src/CAN_STM32h7xx
    LWIP/App
)

add_executable(${PROJECT_NAME}.elf
#    ${FatFs_src}
#    ${Bootloader_src}
#    ${UpdateMechanism_src}
    ${EEPROM_src}
#    ${TraceRecorder_src}
#    ${Tests_src}
#    ${UID_src}
#    ${RTT_src}
#    ${RTP_src}
#    ${WDT_src}
#    ${RTC_src}
#    ${AUDIO_src}
#    ${Flash_src}
#    ${TRNG_src}
    ${Debug_src}
    ${RS232_src}
#    ${CAN_STM32h7xx_src}
#    ${GPIO_STM32h7xx_src}
#    ${GPIO_SC2_BOARD_src}
#    ${RTC_src}
#    ${SRAM_src}
#    ${JSON_src}
#    ${UDP_JSON_src}
#    ${LwIP_src}
#    ${littlefs_src}
#    ${ArduinoJson_src}
#    ${CircularBuffer_src}
    ${FreeRTOS_src}
    ${FreeRTOS_src2}
    ${CMSIS_RTOS_src}
    ${STM32h7xx_HAL_src}
    ${CMSIS_STM32h7xx_src}
#    ${Call_Control_src}
#    ${SNTP_src}
    ${INC_src}
    ${MAIN_src}
    ${startup_src}
    ${LD_src}
    Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c
    Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.h
#    Src/Call_Control/os_timers.cpp
    Middlewares/Third_Party/FreeRTOS/Source/timers.c
    ${HZ_src}
    ${HZ_src2}
    ${HZ_src3}
    ${HZ_src4}
    ${HZ_src5}
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_can.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_eth.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sai.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c
    Drivers/CMSIS_STM32h7xx/DSP/Source/SupportFunctions/arm_copy_q7.c
    Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/port.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c
    )

    add_definitions(-DSTM32F753xx)
    add_definitions(-DUSE_HAL_DRIVER)
    add_definitions(-D__weak=__attribute__\(\(weak\)\))
    add_definitions(-D__packed=__attribute__\(\(__packed__\)\))
    add_definitions(-D__FPU_PRESENT=1)
    add_definitions(-DARM_MATH_CM7)
    add_definitions(-DHAVE_CONFIG_H)
    add_definitions(-D__DCACHE_PRESENT)
    add_definitions(-D__ICACHE_PRESENT)
    add_definitions(-DSC2BOARD)

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
set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINKER_LANGUAGE CXX)
set(HEX_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin)
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
    COMMENT "Building ${HEX_FILE} ")
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
    COMMENT "Building ${BIN_FILE} ")


