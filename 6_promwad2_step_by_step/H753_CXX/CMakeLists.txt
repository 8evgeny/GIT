cmake_minimum_required(VERSION 3.15)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Set ARM compiler
#set(TOOLS_PATH /home/evg/toolchain/gcc-arm-none-eabi-new)
#set(CMAKE_C_COMPILER ${TOOLS_PATH}/bin/arm-none-eabi-gcc)
#set(CMAKE_CXX_COMPILER ${TOOLS_PATH}/bin/arm-none-eabi-g++)
#set(CMAKE_CXX_LINKER_LAUNCHER ${TOOLS_PATH}/bin/arm-none-eabi-g++)
#set(CMAKE_OBJCOPY ${TOOLS_PATH}/bin/arm-none-eabi-objcopy)

project(PW_new_CXX C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_definitions(-DSTM32H753xx)
add_definitions(-DDATA_IN_D2_SRAM)
add_definitions(-DUSE_HAL_DRIVER)
#add_definitions(-D__weak=__attribute__\(\(weak\)\))
#add_definitions(-D__packed=__attribute__\(\(__packed__\)\))
#add_definitions(-D__FPU_PRESENT=1)
#add_definitions(-DARM_MATH_CM7)
#add_definitions(-DHAVE_CONFIG_H)
#add_definitions(-D__DCACHE_PRESENT)
#add_definitions(-D__ICACHE_PRESENT)
#add_definitions(-DSC2BOARD)

include_directories(
    Core/Src
    Core/Inc
    Core/Startup
    Drivers/CMSIS/Include
    Drivers/CMSIS/Device/ST/STM32H7xx/Include
    Drivers/BSP/Components/dp83848
    Drivers/BSP/Components/dp83848
    Drivers/STM32H7xx_HAL_Driver/Inc
    Drivers/STM32H7xx_HAL_Driver/Inc/Legacy
    LWIP/App
    LWIP/Target
    LWIP
    Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
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
    Core/Src/EEPROM
    Middlewares/Third_Party/littlefs
    Core/Src/RS232
    Middlewares/Third_Party/CircularBuffer
    Core/Src/Debug
    )

file (GLOB SOURCES_C      Core/Src/*.c STM32H753IITX_FLASH.ld Core/Startup/startup_stm32h753iitx.s)
file (GLOB SOURCES_CPP    Core/Src/*.cpp )
file (GLOB 83848_SOURCES  Drivers/BSP/Components/dp83848/dp83848.c )
file (GLOB HAL_SRC        Drivers/STM32H7xx_HAL_Driver/Src/*.c)
file (GLOB LWIP_SOURCES   LWIP/App/lwip.c LWIP/Target/ethernetif.c Middlewares/Third_Party/LwIP/src/api/* Middlewares/Third_Party/LwIP/src/apps/mqtt/mqtt.c Middlewares/Third_Party/LwIP/src/core/*     Middlewares/Third_Party/LwIP/src/core/ipv4/*  Middlewares/Third_Party/LwIP/src/netif/* Middlewares/Third_Party/LwIP/src/netif/ppp/* Middlewares/Third_Party/LwIP/system/OS/sys_arch.c)
file (GLOB FreeRTOS_SRC   Middlewares/Third_Party/FreeRTOS/Source/*.c Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/*.c Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c )
file (GLOB EEPROM_SOURCES Core/Src/EEPROM/*.c  Core/Src/EEPROM/*.cpp Middlewares/Third_Party/littlefs/*.c)
file (GLOB RS232_SOURCES  Core/Src/RS232/*.c Core/Src/RS232/*.cpp)
file (GLOB CIRC_BUF_SRC   Middlewares/Third_Party/CircularBuffer/CircularBuffer.tpp Middlewares/Third_Party/CircularBuffer/CircularBuffer.h)
file (GLOB DEBUG_SOURCES Core/Src/Debug/*.c Core/Src/Debug/*.cpp)

add_executable(${PROJECT_NAME}.elf
    ${SOURCES_C}
    ${SOURCES_CPP}
    ${83848_SOURCES}
    ${HAL_SRC}
    ${LWIP_SOURCES}
    ${FreeRTOS_SRC}
#    ${EEPROM_SOURCES}
#    ${RS232_SOURCES}
#    ${CIRC_BUF_SRC}
#    ${DEBUG_SOURCES}
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

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}\
    -Wl,--start-group \
    -Wl,--gc-sections \
    -mcpu=cortex-m7 \
    -DUSE_HAL_DRIVER \
    -DSTM32H753xx \
    -Os \
    -std=c++17 \
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

set(LINKER_SCRIPT_FLASH "${CMAKE_SOURCE_DIR}/STM32H753IITX_FLASH.ld")
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
set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINKER_LANGUAGE CXX)

set(HEX_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
    COMMENT "Building ${HEX_FILE} ")

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
    COMMENT "Building ${BIN_FILE} ")


