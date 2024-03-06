cmake_minimum_required(VERSION 3.15)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(toolchain /home/evg/toolchain/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi)
set(CMAKE_OBJCOPY ${toolchain}/bin/arm-none-eabi-objcopy)

project(Bootloader C CXX ASM)

FILE(STRINGS "version_Bootloader" tmp LIMIT_COUNT 2)
STRING(REGEX MATCHALL "[a-zA-Z0-9_]+[ ]*=[^\n]*(\n|$)" tmp_list "${tmp}")
FOREACH(tmp_x ${tmp_list})
        STRING(REGEX REPLACE "([a-zA-Z0-9_]+)[ ]*=[^\n]*(\n|$)" "\\1" tmp_nm "${tmp_x}")
        STRING(REGEX REPLACE "[a-zA-Z0-9_]+[ ]*=[ ]*([^\n]*)(\n|$)" "\\1" tmp_val "${tmp_x}")
        STRING(STRIP "${tmp_val}" ${tmp_nm})
ENDFOREACH()
MESSAGE("version_Bootloader: ${MAIN_VERSION_FIRMWARE}.${PATCH_VERSION}")
set (MAIN_FW "${MAIN_VERSION_FIRMWARE}")
set (PATCH_FW "${PATCH_VERSION}")
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc/config.h.in config.h @ONLY)

set (fatfs ${CMAKE_HOME_DIRECTORY}/Middlewares/Third_Party/FatFs/src)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}\
    -mcpu=cortex-m4 \
    -std=gnu11 \
    -DUSE_HAL_DRIVER \
    -DSTM32F407xx \
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
#    --specs=nano.specs \

include_directories(
    build
    Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
    Core/Inc
    Drivers/CMSIS/Device/ST/STM32F4xx/Include
    Drivers/STM32F4xx_HAL_Driver/Inc
    Drivers/CMSIS/Core/Include
    Drivers/CMSIS/Include
    FATFS/App
    FATFS/Target
    ${fatfs}
    )

file (GLOB hal_src Drivers/STM32F4xx_HAL_Driver/Src/*.c)
add_library(hal_LIB ${hal_src})

file (GLOB fatfs_src FATFS/App/*.c FATFS/Target/*.c ${fatfs}/*.c ${fatfs}/option/*.c)
add_library(fatfs_LIB ${fatfs_src})

file (GLOB src
    Core/Src/*.c
    Core/Startup/startup_stm32f407vgtx.s
)

set(nameFirmware ${PROJECT_NAME}_${MAIN_VERSION_FIRMWARE}_${PATCH_VERSION})
add_executable(${nameFirmware}
    ${src}
    )

target_compile_definitions(${nameFirmware} PRIVATE fwareName=${nameFirmware})

set_target_properties(${nameFirmware} PROPERTIES LINKER_LANGUAGE CXX)
set(LINKER_SCRIPT_FLASH "${CMAKE_SOURCE_DIR}/STM32F407VGTX_FLASH.ld")
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

target_link_options(${nameFirmware} PRIVATE ${LINKER_FLAGS})

target_link_libraries(${nameFirmware} PUBLIC
    hal_LIB
    fatfs_LIB
    )

if(LFS STREQUAL ON)
    target_link_libraries(${nameFirmware} PUBLIC littlefs_LIB )
endif()

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

#    st-flash --reset write /home/evg/SOFT/Github/GIT/26_Bootloader_F407/build/Bootloader_1_1.hex 0x8000000
#   openocd -f interface/stlink.cfg  -f target/stm32f4x.cfg -c "program /home/evg/SOFT/Github/GIT/26_Bootloader_F407/build/Bootloader_1_1.hex 0x08000000 reset"

