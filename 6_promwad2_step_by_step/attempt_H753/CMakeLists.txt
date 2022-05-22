cmake_minimum_required(VERSION 3.15)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Set ARM compiler
set(TOOLS_PATH /home/evg/toolchain/gcc-arm-none-eabi-new)
#set(CMAKE_C_COMPILER ${TOOLS_PATH}/bin/arm-none-eabi-gcc)
#set(CMAKE_CXX_COMPILER ${TOOLS_PATH}/bin/arm-none-eabi-g++)
set(CMAKE_CXX_LINKER_LAUNCHER ${TOOLS_PATH}/bin/arm-none-eabi-g++)
set(CMAKE_OBJCOPY ${TOOLS_PATH}/bin/arm-none-eabi-objcopy)

project(PW_new_iteration C CXX ASM)

add_definitions(-DSTM32H753xx)
#add_definitions(-DUSE_HAL_DRIVER)
#add_definitions(-D__weak=__attribute__\(\(weak\)\))
#add_definitions(-D__packed=__attribute__\(\(__packed__\)\))
#add_definitions(-D__FPU_PRESENT=1)
#add_definitions(-DARM_MATH_CM7)
#add_definitions(-DHAVE_CONFIG_H)
#add_definitions(-D__DCACHE_PRESENT)
#add_definitions(-D__ICACHE_PRESENT)
#add_definitions(-DSC2BOARD)

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

set (STARTUP_SOURCES ${patchSRC}/../Startup/startup_stm32h753iitx.s )
add_library(STARTUP ${STARTUP_SOURCES} )

include_directories(
    ${patchDRV}/BSP/Components/lan8742
    Drivers/BSP/Components/dp83848)
file (GLOB BSP_SOURCES
    ${patchDRV}/BSP/Components/lan8742/lan8742.c
    Drivers/BSP/Components/dp83848/dp83848.c)
add_library(BSP ${BSP_SOURCES} )

include_directories(
    ${patchDRV}/STM32H7xx_HAL_Driver/Inc
    ${patchDRV}/STM32H7xx_HAL_Driver/Inc/Legacy
    )
file (GLOB HAL_SRC
    ${patchHAL_SRC}/*
    Drivers/STM32H7xx_HAL_Driver/Src/*.c
    )
add_library(STM32H7xx_HAL_Driver ${HAL_SRC})

include_directories(
    LWIP/App
    LWIP/Target
    LWIP
    )
set (LWIP_SOURCES
    LWIP/App/lwip.c
    LWIP/Target/ethernetif.c
#    LWIP/ping.c
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
    Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS
    Middlewares/Third_Party/FreeRTOS/Source/include
    Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
    )

file (GLOB Third_Party_SRC
    Middlewares/Third_Party/LwIP/src/api/*
    Middlewares/Third_Party/LwIP/src/apps/mqtt/mqtt.c
    Middlewares/Third_Party/LwIP/src/core/*
    Middlewares/Third_Party/LwIP/src/core/ipv4/*
    Middlewares/Third_Party/LwIP/src/netif/*
    Middlewares/Third_Party/LwIP/src/netif/ppp/*
    Middlewares/Third_Party/FreeRTOS/Source/*.c
    Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/*.c
    Middlewares/Third_Party/LwIP/system/OS/sys_arch.c
    Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
    Drivers/BSP/Components/lan8742/lan8742.c
    )

add_library(Middlewares
    ${Third_Party_SRC}
    )

#include_directories(EEPROM Middlewares/Third_Party/littlefs)
#file (GLOB EEPROM_SOURCES EEPROM/*.c  EEPROM/*.cpp Middlewares/Third_Party/littlefs/*.c)
#add_library(EEPROM ${EEPROM_SOURCES} )

include_directories(RS232)
file (GLOB RS232_SOURCES RS232/*.c)
add_library(RS232 ${RS232_SOURCES} )

add_executable(${PROJECT_NAME}.elf
#    Drivers/BSP/Components/dp83848/dp83848.c
#    Drivers/BSP/Components/lan8742/lan8742.c
Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c
    ${SOURCES_C}
    )

set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINKER_LANGUAGE CXX)

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
#    EEPROM
    RS232
    )

set(HEX_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
    COMMENT "Building ${HEX_FILE} ")

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
    COMMENT "Building ${BIN_FILE} ")


