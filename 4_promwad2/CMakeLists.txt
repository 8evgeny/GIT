cmake_minimum_required(VERSION 3.15)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Set ARM compiler
#set(TOOLS_PATH /home/evg/toolchain/gcc-arm-none-eabi-new)
#set(CMAKE_C_COMPILER ${TOOLS_PATH}/bin/arm-none-eabi-gcc)
#set(CMAKE_CXX_COMPILER ${TOOLS_PATH}/bin/arm-none-eabi-g++)
set(CMAKE_CXX_LINKER_LAUNCHER ${TOOLS_PATH}/bin/arm-none-eabi-g++)
set(CMAKE_OBJCOPY ${TOOLS_PATH}/bin/arm-none-eabi-objcopy)

project(4_promwad2 C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_definitions(-DSTM32F777xx)

#set(CMAKE_BUILD_TYPE debug)
#set(CMAKE_BUILD_TYPE release)

set (DRV_CMSYS "Drivers/CMSIS_STM32F7xx")
set (DRV_CMSYS_CORE ${DRV_CMSYS}/Core)
set (DRV_CMSYS_DSP ${DRV_CMSYS}/DSP)
set (DRV_CMSYS_Device ${DRV_CMSYS}/Device/ST/STM32F7xx)
set (DRV_HAL "Drivers/STM32F7xx_HAL_Driver")
set (Third_Party "Middlewares/Third_Party")
set (LwIP ${Third_Party}/LwIP)
set (FreeRTOS ${Third_Party}/FreeRTOS/Source)


set (SRC "Src/")
set (BSP ${SRC}BSP)
set (CAN_Command ${SRC}CAN_Command)
set (CAN_STM32Fxx ${SRC}CAN_STM32Fxx)
set (CRC16_CCITT ${SRC}CRC16_CCITT)
set (Call_control ${SRC}Call_control)
set (Call_control_for_SC2_board ${SRC}Call_control_for_SC2_board)
set (EEPROM ${SRC}EEPROM)
set (GPIO_SC2_BOARD ${SRC}GPIO_SC2_BOARD)
set (GPIO_STM32F7xx ${SRC}GPIO_STM32F7xx)
set (JSON ${SRC}JSON)
set (RS232 ${SRC}RS232)
set (SNTP ${SRC}SNTP)
set (SRAM ${SRC}SRAM)
set (UDP_JSON ${SRC}UDP_JSON)
set (c-ringbuf ${SRC}c-ringbuf)

include_directories(
    ${DRV_CMSYS_CORE}/Include
    ${DRV_CMSYS_DSP}/Include
    ${DRV_CMSYS_Device}/Include
    ${DRV_CMSYS}/Include
    ${DRV_HAL}/Inc
    ${DRV_HAL}/Inc/Legacy
    ${Third_Party}/CircularBuffer
    ${FreeRTOS}/CMSIS_RTOS_V2
    ${FreeRTOS}/include
    ${FreeRTOS}/portable/GCC/ARM_CM7/r0p1
    ${LwIP}/src/include
    ${LwIP}/system
    Src/CAN_STM32F7xx
    Inc
    startup
    ${BSP}
    ${Bootloader}
    ${Bootloader}/AES128
    ${Bootloader}/MD5
    ${CAN_Command}
    ${CAN_STM32Fxx}
    ${CRC16_CCITT}
    ${Call_control}
    ${Call_control_for_SC2_board}
    ${EEPROM}
    ${GPIO_SC2_BOARD}
    ${GPIO_STM32F7xx}
    ${JSON}
    ${SNTP}
    ${SRAM}
    ${UDP_JSON}
    ${c-ringbuf}
    )

set (FatFs ${Third_Party}/FatFs)
file (GLOB_RECURSE FatFs_src
    ${FatFs}/src/*.c
    ${FatFs}/src/option/syscall.c
    ${FatFs}/src/option/unicode.c
    )
include_directories(
    ${FatFs}
    ${FatFs}/src
    )

set (Bootloader ${SRC}Bootloader)
file (GLOB_RECURSE Bootloader_src
    ${Bootloader}/*.cpp
    ${Bootloader}/MD5/*.cpp
    ${Bootloader}/AES128/*.c
    )
include_directories(
    ${Bootloader}
    ${Bootloader}/MD5
    ${Bootloader}/AES128
    )

set (UpdateMechanism  ${SRC}UpdateMechanism)
file (GLOB UpdateMechanism_src ${UpdateMechanism}/*.cpp)
include_directories(${UpdateMechanism})

set (EEPROM  ${SRC}EEPROM)
file (GLOB_RECURSE EEPROM_src ${EEPROM}/*.cpp ${EEPROM}/*.c )
include_directories(${EEPROM})

set (TraceRecorder  "Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder")
file (GLOB_RECURSE TraceRecorder_src
    ${TraceRecorder}/*.c
    ${TraceRecorder}/streamports/TCPIP/*.c
    )
include_directories(
    ${TraceRecorder}/streamports/TCPIP/include
    ${TraceRecorder}/include
    ${TraceRecorder}/config
    )

set (Tests  ${SRC}Tests)
file (GLOB Tests_src ${Tests}/*.cpp )
include_directories(${Tests})

set (UID  ${SRC}uid)
file (GLOB UID_src ${UID}/*.cpp )
include_directories(${UID})

set (RTP  ${SRC}RTP)
file (GLOB RTP_src ${RTP}/*.cpp )
include_directories( ${RTP} )

set (WDT  ${SRC}WDT)
file (GLOB WDT_src ${WDT}/*.cpp )
include_directories( ${WDT} )

set (RTC  ${SRC}RTC)
file (GLOB RTC_src ${WDT}/*.cpp )
include_directories( ${RTC} )

set (AUDIO  ${SRC}AUDIO)
file (GLOB AUDIO_src ${AUDIO}/*.cpp )
include_directories( ${AUDIO} )

set (Flash  ${SRC}flash)
file (GLOB Flash_src ${Flash}/*.cpp )
include_directories( ${Flash} )

set (TRNG  ${SRC}TRNG)
file (GLOB TRNG_src ${TRNG}/*.cpp )
include_directories( ${TRNG} )


file (GLOB_RECURSE SOURCES
    ${BSP}/*.c
    ${Bootloader}/*.cpp
    ${CAN_Command}/*.cpp
    ${CAN_STM32Fxx}/*.cpp
    ${CRC16_CCITT}/*.cpp
    ${Call_control}/*.cpp
    ${Call_control_for_SC2_board}/*.cpp
    ${EEPROM}/*.cpp
    ${EEPROM}/*.c
    ${GPIO_SC2_BOARD}/*.cpp
    ${GPIO_STM32F7xx}/*.cpp
    ${JSON}/*.cpp
    ${RS232}/*.cpp
    ${RS232}/*.c
    ${SNTP}/*.cpp
    ${SRAM}/*.cpp
    ${UDP_JSON}/*.cpp
    STM32F746NGHx_FLASH.ld
    )


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


add_executable(${PROJECT_NAME}.elf
    ${SOURCES}
    ${FatFs_src}
    ${Bootloader_src}
    ${UpdateMechanism_src}
    ${EEPROM_src}
    ${TraceRecorder_src}
    ${Tests_src}
    ${UID_src}
    ${RTP_src}
    ${WDT_src}
    ${RTC_src}
    ${AUDIO_src}
    ${Flash_src}
    ${TRNG_src}
    )


message("SOURCES")
message(${SOURCES})

#set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINKER_LANGUAGE C)
set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINKER_LANGUAGE CXX)

set(LINKER_SCRIPT_FLASH "${CMAKE_SOURCE_DIR}/STM32F746NGHx_FLASH.ld")

set(LINKER_FLAGS "SHELL: \
    -T${LINKER_SCRIPT_FLASH} \
    -mcpu=cortex-m7 \
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

target_link_options(${PROJECT_NAME}.elf PRIVATE ${LINKER_FLAGS})


#target_link_libraries(${PROJECT_NAME}.elf PUBLIC
#    CORE
#    STARTUP
#    BSP
#    STM32H7xx_HAL_Driver
#    LWIP
#    Middlewares
#    )

set(HEX_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
    COMMENT "Building ${HEX_FILE} ")

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
    COMMENT "Building ${BIN_FILE} ")


