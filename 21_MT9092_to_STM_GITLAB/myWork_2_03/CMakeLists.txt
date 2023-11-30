cmake_minimum_required(VERSION 3.15)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(TOOLS_PATH /home/evg/toolchain/gcc-arm-none-eabi-new)
set(CMAKE_OBJCOPY ${TOOLS_PATH}/bin/arm-none-eabi-objcopy)

project(Firmware_DX C CXX ASM)

set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} \
    -mcpu=cortex-m4 \
    -g3 \
    -DDEBUG \
    -c \
    -x assembler-with-cpp \
    --specs=nano.specs \
    -mfpu=fpv4-sp-d16 \
    -mfloat-abi=hard \
    -mthumb \
    ")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
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
    -fstack-usage \
    --specs=nano.specs \
    -mfpu=fpv4-sp-d16 \
    -mfloat-abi=hard \
    -mthumb \
    -Xlinker -Map=${PROJECT_NAME}.map \
    ")
#-Wall

#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

include_directories(
    Inc
    Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
    Drivers/CMSIS/Include
    Drivers/CMSIS/Device/ST/STM32F4xx/Include
    Drivers/STM32F4xx_HAL_Driver/Inc
    Drivers/CMSIS/Core/Include
    )

file (GLOB hal_src Drivers/STM32F4xx_HAL_Driver/Src/*.c)
list(REMOVE_ITEM hal_src
    ${CMAKE_HOME_DIRECTORY}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc.c
    ${CMAKE_HOME_DIRECTORY}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_timebase_rtc_alarm_template.c
    ${CMAKE_HOME_DIRECTORY}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_timebase_rtc_wakeup_template.c
    )
add_library(hal_LIB ${hal_src})

file (GLOB src Src/*.c
    startup_stm32f429vgtx.s
    )
add_executable(${PROJECT_NAME}.elf ${src})

set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINKER_LANGUAGE CXX)
set(LINKER_SCRIPT_FLASH "${CMAKE_SOURCE_DIR}/STM32F429VGTX_FLASH.ld")
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
#    COMMAND st-flash --reset write ${BIN_FILE} 0x8000000
#    COMMENT "Write Flash ")
