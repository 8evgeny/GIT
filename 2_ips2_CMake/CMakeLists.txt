cmake_minimum_required(VERSION 3.15)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Set ARM compiler
set(TOOLS_PATH /home/evg/toolchain/gcc-arm-none-eabi-new)
set(CMAKE_C_COMPILER ${TOOLS_PATH}/bin/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER ${TOOLS_PATH}/bin/arm-none-eabi-g++)

project(display_cmake_release C CXX)

#set(CMAKE_BUILD_TYPE debug)
set(CMAKE_BUILD_TYPE release)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}\
    -mcpu=cortex-m3 \
    -mthumb \
    -O2 \
    -ffunction-sections \
    -fdata-sections \
    -Wall \
    -Wextra \
    -Wshadow \
    -Wredundant-decls \
    -Wno-missing-field-initializers \
    -Wconversion \
    -Wno-packed-bitfield-compat \
    -pipe \
    -std=c99 \
    -DUSE_MDR1986VE9x \
    -D__STARTUP_CLEAR_BSS \
    -D__START=main \
    --specs=nosys.specs \
    --specs=nano.specs \
    ")


#-fno-exceptions \
#-D__HEAP_SIZE=0x0C00 \
#-D__STACK_SIZE=0x0400 \
#-mfloat-abi=soft \
#-fno-strict-aliasing \

#-nostdli
#-xc
#--target=arm-arm-none-eabi
#-c
#-fno-rtti
#-funsigned-char
#-fshort-enums
#-fshort-wchar

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
    -fno-rtti \
    -fno-exceptions \
    -fno-threadsafe-statics \
    -DUSE_MDR1986VE9x \
    -D__STARTUP_CLEAR_BSS \
    --specs=nano.specs \
    --specs=nosys.specs \
    -mcpu=cortex-m3 \
    -std=c++11 \
    -Wl,-Map=${PROJECT_NAME}.map \
    -Wl,--print-memory-usage \
    -Wl,--defsym=__STACK_SIZE=0x0100 \
    -Wl,--defsym=__HEAP_SIZE=0x0000 \
    -Wl,--gc-sections \
    -ffreestanding \
    ")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
#    -T /home/evg/SOFT/Github/Arm/Milandr_MCU_1986x_Standard_Peripherals_Library/CMSIS/CM3/DeviceSupport/MDR32F9Qx/startup/gcc/MDR32F9Qx.ld \
#    ")

#-mthumb \
#-O3 \
#-o prog \
#-D_GNU_SOURCE \
#-L./libmine \

#-rdynamic
#-nostdlib

set(CMAKE_LINK_LIBRARY_FLAG "${CMAKE_LINK_LIBRARY_FLAG} \
    -ffunction-sections \
    -fdata-sections \
    -fno-strict-aliasing \
    -mfloat-abi=soft \
    -mthumb \
    -mcpu=cortex-m3 \
    --specs=nosys.specs \
    --specs=nano.specs \
    -ffreestanding \
    -nostartfiles \
    -T /home/evg/SOFT/Github/Arm/Milandr_MCU_1986x_Standard_Peripherals_Library/CMSIS/CM3/DeviceSupport/MDR32F9Qx/startup/gcc/MDR32F9Qx.ld
    ")

include_directories(
    ../MDR1986BExx/2.0.3/Libraries/CMSIS/MDR32Fx/DeviceSupport/MDR1986VE9x/inc
    ../MDR1986BExx/2.0.3/Libraries/CMSIS/MDR32Fx/DeviceSupport/MDR1986VE9x/startup/arm
    ../Milandr_MCU_1986x_Standard_Peripherals_Library/CMSIS/CM3/DeviceSupport/MDR32F9Qx/startup/gcc
    ../MDR1986BExx/2.0.3/Libraries/CMSIS/MDR32Fx/CoreSupport/CM3
    ../MDR1986BExx/2.0.3/Libraries/SPL/MDR32Fx
    ../MDR1986BExx/2.0.3/Libraries/SPL/MDR32Fx/inc
    ../MDR1986BExx/2.0.3/Libraries/SPL/MDR32Fx/inc/USB_Library
    ../MDR1986BExx/2.0.3/Libraries/MDR32Fx/inc
    ../MDR1986BExx/2.0.3/Libraries/SPL/MDR32Fx/src
    /home/evg/toolchain/gcc-arm-none-eabi-new/arm-none-eabi
    include
    src
    )

set (SOURCES_C
    src/gpio.c
    src/mt20s4.c
    src/rcc_delay.c
    src/var.c
    )

set (SOURCES_CPP
    src/main.cpp
    src/logic.cpp
    src/usb.cpp
    src/checkLCD_BUZZER.cpp
    )

set (SOURCES_SPL
    ../MDR1986BExx/2.0.3/Libraries/SPL/MDR32Fx/src/MDR32F9Qx_iwdg.c
    ../MDR1986BExx/2.0.3/Libraries/SPL/MDR32Fx/src/MDR32F9Qx_port.c
    ../MDR1986BExx/2.0.3/Libraries/SPL/MDR32Fx/src/MDR32F9Qx_rst_clk.c
    ../MDR1986BExx/2.0.3/Libraries/SPL/MDR32Fx/src/MDR32F9Qx_timer.c
    ../MDR1986BExx/2.0.3/Libraries/SPL/MDR32Fx/src/MDR32F9Qx_usb.c
    )

set (SOURCES_USB
    ../MDR1986BExx/2.0.3/Libraries/SPL/MDR32Fx/src/USB_Library/MDR32F9Qx_usb_CDC.c
    ../MDR1986BExx/2.0.3/Libraries/SPL/MDR32Fx/src/USB_Library/MDR32F9Qx_usb_device.c
    )

set (SOURCES_DEVICE_SUPPORT
    ../MDR1986BExx/2.0.3/Libraries/CMSIS/MDR32Fx/DeviceSupport/MDR1986VE9x/startup/arm/system_MDR32F9Qx.c
    )

set (STARTUP_DEVICE_SUPPORT
    ../Milandr_MCU_1986x_Standard_Peripherals_Library/CMSIS/CM3/DeviceSupport/MDR32F9Qx/startup/gcc/startup_MDR32F9Qx.S
    )

add_library(CMSIS
    ${SOURCES_SPL}
    ${SOURCES_USB}
    ${SOURCES_DEVICE_SUPPORT}
    ${STARTUP_DEVICE_SUPPORT}
    )

add_executable(${PROJECT_NAME}.elf
    ${SOURCES_C}
    ${SOURCES_CPP}
    )

set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(${PROJECT_NAME}.elf PUBLIC
    CMSIS
#    m
#    gcc
    )

set(HEX_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
    COMMENT "Building ${HEX_FILE} ")

#add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
#    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
#    COMMENT "Building ${BIN_FILE} ")

#include(${CMAKE_SOURCE_DIR}/conanbuildinfo.cmake)
#include(${CMAKE_SOURCE_DIR}/conanbuildinfo.cmake)
#conan_basic_setup()


#target_link_libraries(${PROJECT_NAME}
#    ${CONAN_LIBS}
#    )
