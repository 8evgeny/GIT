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

set (Board_name SC2)
set (HeapVersion 4)
#set (HeapVersion 5)
#set (Board_name SC4)
add_definitions(-DSTM32F777xx)


#set(CMAKE_BUILD_TYPE debug)
#set(CMAKE_BUILD_TYPE release)

set (DRV_CMSYS "Drivers/CMSIS_STM32F7xx")
set (DRV_CMSYS_CORE ${DRV_CMSYS}/Core)
set (DRV_CMSYS_DSP ${DRV_CMSYS}/DSP)
set (DRV_CMSYS_Device ${DRV_CMSYS}/Device/ST/STM32F7xx)
set (DRV_HAL "Drivers/STM32F7xx_HAL_Driver")
set (Third_Party "Middlewares/Third_Party")

set (SRC "Src/")
set (BSP ${SRC}BSP)
set (CAN_Command ${SRC}CAN_Command)
set (CRC16_CCITT ${SRC}CRC16_CCITT)
set (Call_control ${SRC}Call_control)
set (Call_control_for_SC2_board ${SRC}Call_control_for_SC2_board)
set (SNTP ${SRC}SNTP)
set (c-ringbuf ${SRC}c-ringbuf)

include_directories(
    ${DRV_CMSYS_CORE}/Include
    ${DRV_CMSYS_DSP}/Include
    ${DRV_CMSYS_Device}/Include
    ${DRV_CMSYS}/Include
    ${DRV_HAL}/Inc
    ${DRV_HAL}/Inc/Legacy
    ${FreeRTOS}/CMSIS_RTOS_V2
    ${FreeRTOS}/include
    ${FreeRTOS}/portable/GCC/ARM_CM7/r0p1
    Src/CAN_STM32F7xx
    Inc
    startup
    ${BSP}
    ${Bootloader}/AES128
    ${Bootloader}/MD5
    ${CAN_Command}
    ${CRC16_CCITT}
    ${Call_control}
    ${Call_control_for_SC2_board}
    ${SNTP}
    ${c-ringbuf}
    )

set (FatFs ${Third_Party}/FatFs)
file (GLOB_RECURSE FatFs_src ${FatFs}/src/*.c ${FatFs}/src/option/syscall.c ${FatFs}/src/option/unicode.c )
include_directories(${FatFs} ${FatFs}/src )

set (Bootloader ${SRC}Bootloader)
file (GLOB_RECURSE Bootloader_src ${Bootloader}/*.cpp ${Bootloader}/MD5/*.cpp ${Bootloader}/AES128/*.c )
include_directories(${Bootloader} ${Bootloader}/MD5 ${Bootloader}/AES128 )

set (UpdateMechanism  ${SRC}UpdateMechanism)
file (GLOB UpdateMechanism_src ${UpdateMechanism}/*.cpp)
include_directories(${UpdateMechanism})

set (EEPROM  ${SRC}EEPROM)
file (GLOB_RECURSE EEPROM_src ${EEPROM}/*.cpp ${EEPROM}/*.c )
include_directories(${EEPROM})

set (TraceRecorder  "Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder")
file (GLOB_RECURSE TraceRecorder_src ${TraceRecorder}/*.c ${TraceRecorder}/streamports/TCPIP/*.c )
include_directories(${TraceRecorder}/streamports/TCPIP/include ${TraceRecorder}/include ${TraceRecorder}/config )

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

set (Debug  ${SRC}Debug)
file (GLOB Debug_src ${Debug}/*.cpp ${Debug}/*.c)
include_directories( ${Debug} )

set (RS232  ${SRC}RS232)
file (GLOB RS232_src ${RS232}/*.cpp ${RS232}/*.c)
include_directories( ${RS232} )

if(${Board_name} STREQUAL SC4)
    set (CAN_STM32F7xx  ${SRC}CAN_STM32F7xx)
    file (GLOB CAN_STM32F7xx_src ${CAN_STM32F7xx}/*.cpp )
    include_directories( ${CAN_STM32F7xx} )
endif()

if(${Board_name} STREQUAL SC4)
    set (GPIO_STM32F7xx  ${SRC}GPIO_STM32F7xx)
    file (GLOB GPIO_STM32F7xx_src ${GPIO_STM32F7xx}/*.cpp )
    include_directories( ${GPIO_STM32F7xx} )
endif()

if(${Board_name} STREQUAL SC2)
    set (GPIO_SC2_BOARD  ${SRC}GPIO_SC2_BOARD)
    file (GLOB GPIO_SC2_BOARD_src ${GPIO_SC2_BOARD}/*.cpp )
    include_directories( ${GPIO_SC2_BOARD} )
endif()

set (RTC  ${SRC}RTC)
file (GLOB RTC_src ${RTC}/*.cpp )
include_directories( ${RTC} )

set (SRAM  ${SRC}SRAM)
file (GLOB SRAM_src ${SRAM}/*.cpp )
include_directories( ${SRAM} )

set (JSON  ${SRC}JSON)
file (GLOB JSON_src ${JSON}/*.cpp )
include_directories( ${JSON} )

set (UDP_JSON  ${SRC}UDP_JSON)
file (GLOB UDP_JSON_src ${UDP_JSON}/*.cpp )
include_directories( ${UDP_JSON} )

set (LwIP ${Third_Party}/LwIP)
file (GLOB_RECURSE LwIP_src
    ${LwIP}/src/netif/*.c
    ${LwIP}/src/api/*.c
    ${LwIP}/src/apps/sntp/*.c
    ${LwIP}/src/core/*.c
    ${LwIP}/src/api/*.c
    ${LwIP}/src/core/ipv4/*.c
    ${LwIP}/system/OS/*.c
    )
include_directories(
    ${LwIP}/src/include/*/*.h
    ${LwIP}/src/include/*/*/*.h
    ${LwIP}/system/*.h
    ${LwIP}/system/OS/*.h
    ${LwIP}/system/*.h
    )

set (littlefs ${Third_Party}/littlefs)
file (GLOB littlefs_src ${littlefs}/*.c )
include_directories( ${littlefs} )

set (ArduinoJson ${Third_Party}/ArduinoJson)
include_directories( ${ArduinoJson} )

set (CircularBuffer ${Third_Party}/CircularBuffer)
file (GLOB CircularBuffer_src ${CircularBuffer}/*.c )
include_directories( ${CircularBuffer} )

if(${HeapVersion} EQUAL 4)
    set (FreeRTOS ${Third_Party}/FreeRTOS)
    file (GLOB FreeRTOS_src ${FreeRTOS}/Source/portable/MemMang/heap_4.c )
endif()

if(${HeapVersion} EQUAL 5)
    set (FreeRTOS ${Third_Party}/FreeRTOS)
    file (GLOB FreeRTOS_src ${FreeRTOS}/Source/portable/MemMang/heap_5.c )
endif()

set (CMSIS_RTOS ${Third_Party}/FreeRTOS/CMSIS_RTOS)
file (GLOB CMSIS_RTOS_src ${CMSIS_RTOS}/*.c )
include_directories( ${CMSIS_RTOS} )
#STM32F7xx_HAL




file (GLOB_RECURSE SOURCES
    ${BSP}/*.c
    ${Bootloader}/*.cpp
    ${CAN_Command}/*.cpp
    ${CRC16_CCITT}/*.cpp
    ${Call_control}/*.cpp
    ${Call_control_for_SC2_board}/*.cpp
    ${SNTP}/*.cpp
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
    ${CircularBuffer_src}
    ${CMSIS_RTOS_src}
    ${}
    ${}
    ${}
    ${}
    ${}
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


