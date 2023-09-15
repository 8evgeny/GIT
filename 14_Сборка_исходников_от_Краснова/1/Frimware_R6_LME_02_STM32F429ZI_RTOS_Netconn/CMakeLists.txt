cmake_minimum_required(VERSION 3.15)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Set ARM compiler
set(TOOLS_PATH /home/evg/toolchain/gcc-arm-none-eabi-new)
#set(CMAKE_C_COMPILER ${TOOLS_PATH}/bin/arm-none-eabi-gcc)
#set(CMAKE_CXX_COMPILER ${TOOLS_PATH}/bin/arm-none-eabi-g++)
#set(CMAKE_CXX_LINKER_LAUNCHER ${TOOLS_PATH}/bin/arm-none-eabi-g++)
set(CMAKE_OBJCOPY ${TOOLS_PATH}/bin/arm-none-eabi-objcopy)

project(14_Build_code_Krasnov1 C CXX ASM)
#set(CMAKE_BUILD_TYPE debug)
#set(CMAKE_BUILD_TYPE release)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}\
    -mcpu=cortex-m4 \
    -std=gnu11 \
    -g3 \
    -DDEBUG \
    -DUSE_FULL_LL_DRIVER \
    -DUSE_HAL_DRIVER \
    -DSTM32F429xx \
    -c \
    -O0 \
    -ffunction-sections \
    -fdata-sections \
    -Wall \
    -fstack-usage \
    --specs=nano.specs \
    -mfpu=fpv4-sp-d16 \
    -mfloat-abi=hard \
    -mthumb \
    -Xlinker -Map=${PROJECT_NAME}.map \
    ")

include_directories(
    Inc
    Middlewares/Third_Party/LwIP/src/include
    Middlewares/Third_Party/LwIP/system
    Middlewares/Third_Party/FreeRTOS/Source/include
    Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS
    Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
    Middlewares/Third_Party/LwIP/src/include/netif/ppp
    Middlewares/Third_Party/LwIP/src/apps/http
    Middlewares/Third_Party/LwIP/src/include/lwip
    Middlewares/Third_Party/LwIP/src/include/lwip/apps
    Middlewares/Third_Party/LwIP/src/include/lwip/priv
    Middlewares/Third_Party/LwIP/src/include/lwip/prot
    Middlewares/Third_Party/LwIP/src/include/netif
    Middlewares/Third_Party/LwIP/src/include/compat/posix
    Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa
    Middlewares/Third_Party/LwIP/src/include/compat/posix/net
    Middlewares/Third_Party/LwIP/src/include/compat/posix/sys
    Middlewares/Third_Party/LwIP/src/include/compat/stdc
    Middlewares/Third_Party/LwIP/system/arch
    Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
    Drivers/CMSIS/Include
    Drivers/CMSIS/Device/ST/STM32F4xx/Include
    Drivers/STM32F4xx_HAL_Driver/Inc
    Drivers/CMSIS/Core/Include
    )

file (GLOB hal_src Drivers/STM32F4xx_HAL_Driver/Src/*.c)
add_library(hal_LIB ${hal_src})

file (GLOB src Src/*.c
    startup_stm32f429zitx.s
    )
add_executable(${PROJECT_NAME}.elf ${src})

set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINKER_LANGUAGE CXX)
set(LINKER_SCRIPT_FLASH "${CMAKE_SOURCE_DIR}/STM32F429ZITX_FLASH.ld")
set(LINKER_FLAGS "SHELL: \
    -T${LINKER_SCRIPT_FLASH} \
    -mcpu=cortex-m4 \
    --specs=nosys.specs \
    -Wl,-Map=${PROJECT_NAME}.map \
    -Wl,--gc-sections \
    -static \
    --specs=nano.specs \
    -Wl,--gc-sections,--print-memory-usage \
    -mfpu=fpv5-d16 \
    -mfloat-abi=hard \
    -mthumb
    -Wl,--start-group \
    -lc \
    -lm \
    -Wl,--end-group \
    ")

target_link_options(${PROJECT_NAME}.elf PRIVATE ${LINKER_FLAGS})

target_link_libraries(${PROJECT_NAME}.elf PUBLIC
    hal_LIB
    )

set(HEX_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
    COMMENT "Building ${HEX_FILE} ")

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
    COMMENT "Building ${BIN_FILE} ")

#add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
#    COMMAND st-flash erase
#    COMMENT "Erasing . . .")

#add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
#    COMMAND st-flash --reset write ${BIN_FILE} 0x8000000
#    COMMENT "Write Flash ")
