cmake_minimum_required(VERSION 3.15)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Set ARM compiler
set(toolchain /home/evg/toolchain/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi)
#set(TOOLS_PATH /home/evg/toolchain/gcc-arm-none-eabi-new)
#set(CMAKE_C_COMPILER ${toolchain}/bin/arm-none-eabi-gcc)
#set(CMAKE_CXX_COMPILER ${toolchain}/bin/arm-none-eabi-g++)
#set(CMAKE_CXX_LINKER_LAUNCHER ${toolchain}/bin/arm-none-eabi-g++)
#set(CMAKE_OBJCOPY ${toolchain}/bin/arm-none-eabi-objcopy)
set(CMAKE_OBJCOPY ${toolchain}/bin/arm-none-eabi-objcopy)

project(Bootloader2 C CXX ASM)
#    -DSTM32F407xx \
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}\
    -mcpu=cortex-m4 \
    -std=gnu11 \
    -DSTM32F4x7 \
    -DUSE_STM32F4_DISCOVERY \
    -c \
    -Os \
    -g \
    -ffunction-sections \
    -fdata-sections \
    -Wall \
    -fstack-usage \
    -mfpu=fpv4-sp-d16 \
    -mfloat-abi=hard \
    -mthumb \
    -Xlinker -Map=${PROJECT_NAME}.map \
    ")

include_directories(
    .
    drivers/inc
    fatfs
    libs/CMSIS/Include
    libs/Device/STM32F4xx/Include
    libs/STM32F4xx_StdPeriph_Driver/inc
    sd
    src
    )

file (GLOB drv_src drivers/src/*.c)
#add_library(drv_LIB ${drv_src})

file (GLOB fatfs_src fatfs/*.c)
#add_library(fatfs_LIB ${fatfs_src})

file (GLOB StdPeriph_src libs/STM32F4xx_StdPeriph_Driver/src/*.c)
#add_library(StdPeriph_LIB ${StdPeriph_src})

file (GLOB SD_src sd/*.c)
#add_library(SD_LIB ${SD_src})

file (GLOB src main.c src/*.c)

set(nameFirmware ${PROJECT_NAME})
add_executable(${nameFirmware}
    ${drv_src}
    ${fatfs_src}
    ${StdPeriph_src}
    ${SD_src}
    ${src}
    src/startup_stm32f4xx.s
)

set_target_properties(${nameFirmware} PROPERTIES LINKER_LANGUAGE CXX)
set(LINKER_SCRIPT_FLASH "${CMAKE_SOURCE_DIR}/src/stm32_flash.ld")
set(LINKER_FLAGS "SHELL: \
    -T${LINKER_SCRIPT_FLASH} \
    -mcpu=cortex-m4 \
    --specs=nosys.specs \
    -Wl,-Map=${nameFirmware}.map \
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

#target_link_options(${nameFirmware} PRIVATE ${LINKER_FLAGS})

#target_link_libraries(${nameFirmware} PUBLIC
#    drv_LIB
#    fatfs_LIB
#    StdPeriph_LIB
#    SD_LIB
#    )

set(HEX_FILE ${CMAKE_BINARY_DIR}/${nameFirmware}.hex)
set(BIN_FILE ${CMAKE_BINARY_DIR}/${nameFirmware}.bin)

add_custom_command(TARGET ${nameFirmware} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${nameFirmware}> ${HEX_FILE}
    COMMENT "Building ${HEX_FILE} ")

add_custom_command(TARGET ${nameFirmware} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${nameFirmware}> ${BIN_FILE}
    COMMENT "Building ${BIN_FILE} ")

#add_custom_command(TARGET ${nameFirmware} POST_BUILD
#    COMMAND st-flash erase
#    COMMENT "Erasing . . .")

#add_custom_command(TARGET ${nameFirmware} POST_BUILD
#    COMMAND st-flash --reset write ${BIN_FILE} 0x8000000
#    COMMENT "Write Flash ")
