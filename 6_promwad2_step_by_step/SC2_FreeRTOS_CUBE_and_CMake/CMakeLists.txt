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

#set(CMAKE_BUILD_TYPE debug)
#set(CMAKE_BUILD_TYPE release)

set (patchSRC "Core/Src")
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
#    --specs=nano.specs \

include_directories(
    ${patchSRC}/../Inc
    ${patchSRC}
    ${patchSRC}/../Startup
    )
file (GLOB SOURCES_C ${patchSRC}/*.c STM32H753IITX_FLASH.ld)

file (GLOB SOURCES_CPP ${patchSRC}/*.cpp)

include_directories(
    Core/Src
    ${patchDRV}/CMSIS/Include
    ${patchDRV}/CMSIS/Device/ST/STM32H7xx/Include
    Drivers/BSP/Components/dp83848
    )
file (GLOB CORE_SOURCES
    Core/Src/*.c
    Drivers/BSP/Components/dp83848/dp83848.c
    LWIP/Target/ethernetif.c
    )
add_library(CORE ${CORE_SOURCES} )




#ХЗ не работает
#set (GPIO_SC2  ${patchSRC}/GPIO_SC2_BOARD)
#include_directories( ${GPIO_SC2_BOARD} )
#file (GLOB GPIO_SC2_BOARD_SOURCES ${GPIO_SC2}/*.cpp )
#add_library(GPIO_SC2_BOARD_LIB ${GPIO_SC2_BOARD_SOURCES} )

#set (RS232  ${patchSRC}/RS232)
#include_directories( ${RS232} )
#file (GLOB RS232_src ${RS232}/*.cpp ${RS232}/*.c ${RS232}/*.h )
#add_library(RS232_LIB ${RS232_src} )

set (EEPROM  ${patchSRC}/EEPROM)
include_directories(${EEPROM})
file (GLOB_RECURSE EEPROM_src ${EEPROM}/*.cpp ${EEPROM}/*.c ${EEPROM}/*.h )
add_library(EEPROM_LIB ${EEPROM_src} )

set (littlefs Middlewares/Third_Party/littlefs)
include_directories( ${littlefs} )
file (GLOB littlefs_src ${littlefs}/*.c ${littlefs}/*.h  )
add_library(littlefs_LIB ${littlefs_src} )

set (CircularBuffer Middlewares/Third_Party/CircularBuffer)
include_directories( ${CircularBuffer} )
#file (GLOB CircularBuffer_src ${CircularBuffer}/*.c  ${CircularBuffer}/*.h )



#set (JSON  ${patchSRC}/JSON)
#include_directories( ${JSON} )
#file (GLOB JSON_src ${JSON}/*.cpp ${JSON}/*.h )
#add_library(JSON_LIB ${JSON_src} )










set (STARTUP_SOURCES ${patchSRC}/../Startup/startup_stm32h753iitx.s )
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

set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINKER_LANGUAGE C)

set(LINKER_SCRIPT_FLASH "${CMAKE_SOURCE_DIR}/STM32H753IITX_FLASH.ld")
set(LINKER_SCRIPT_RAM "${CMAKE_SOURCE_DIR}/STM32H753IITX_RAM.ld")
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
#    GPIO_SC2_BOARD_LIB
#    RS232_LIB
    EEPROM_LIB
    littlefs_LIB
#    JSON_LIB
    )

set(HEX_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
    COMMENT "Building ${HEX_FILE} ")

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
    COMMENT "Building ${BIN_FILE} ")


