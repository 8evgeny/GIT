cmake_minimum_required(VERSION 3.15)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Set ARM compiler
set(TOOLS_PATH /home/evg/toolchain/gcc-arm-none-eabi-new)
set(CMAKE_C_COMPILER ${TOOLS_PATH}/bin/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER ${TOOLS_PATH}/bin/arm-none-eabi-g++)
set(CMAKE_CXX_LINKER_LAUNCHER ${TOOLS_PATH}/bin/arm-none-eabi-g++)
set(CMAKE_OBJCOPY ${TOOLS_PATH}/bin/arm-none-eabi-objcopy)

project(Example_1 C CXX ASM)

#set(CMAKE_BUILD_TYPE debug)
#set(CMAKE_BUILD_TYPE release)

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

include_directories( Inc
    Middlewares/Third_Party/LwIP/src/include
    Middlewares/Third_Party/LwIP/system
    STM32H7xx_HAL_Driver/Inc
    CMSIS/Device/ST/STM32H7xx/Include
    CMSIS/Include
    STM32H7xx_Nucleo
    Components/lan8742)
file (GLOB SOURCES_C Src/*.c
    STM32CubeIDE/Application/User/*.c
    CMSIS/Device/ST/STM32H7xx/Source/Templates/gcc/startup_stm32h753xx.s
    Middlewares/Third_Party/LwIP/src/core/*.c
    STM32H7xx_HAL_Driver/Src/Legacy/*.c
#    STM32H7xx_HAL_Driver/Src/*.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_eth.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c
    Middlewares/Third_Party/LwIP/src/core/ipv4/*.c
    Middlewares/Third_Party/LwIP/src/netif/*.c
    STM32H7xx_Nucleo/stm32h7xx_nucleo.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c
    )


add_executable(${PROJECT_NAME}.elf
    ${SOURCES_C}
    Components/lan8742/lan8742.c
    STM32CubeIDE/STM32H723ZGTX_FLASH.ld
    )

set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINKER_LANGUAGE C)

set(LINKER_SCRIPT_FLASH "${CMAKE_SOURCE_DIR}/CMSIS/Device/ST/STM32H7xx/Source/Templates/gcc/linker/stm32h755xx_flash_CM4.ld")
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
#    )

set(HEX_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
    COMMENT "Building ${HEX_FILE} ")

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
    COMMENT "Building ${BIN_FILE} ")


