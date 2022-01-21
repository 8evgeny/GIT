cmake_minimum_required(VERSION 3.15)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Set ARM compiler
set(TOOLS_PATH /home/evg/toolchain/gcc-arm-none-eabi-new)
set(CMAKE_C_COMPILER ${TOOLS_PATH}/bin/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER ${TOOLS_PATH}/bin/arm-none-eabi-g++)

project(ips2 C ASM)

set(CMAKE_BUILD_TYPE debug)
#set(CMAKE_BUILD_TYPE release)

set (patchSRC "../2_ips2/ips_sc4_pdo/Core/Src")
set (patchDRV "../2_ips2/ips_sc4_pdo/Drivers")
set (patchHAL_SRC "${patchDRV}/STM32H7xx_HAL_Driver/Src")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}\
    -mcpu=cortex-m7 \
    -mfloat-abi=hard \
    -mfpu=fpv5-sp-d16 \
    -mthumb \
    -Xlinker \
    --gc-sections \
    -specs=nosys.specs \
    -specs=nano.specs \
    -T ../2_ips2/ips_sc4_pdo/SSTM32H753IITX_RAM.ld \
    -T ../2_ips2/ips_sc4_pdo/STM32H753IITX_FLASH.ld \
    ")

set(CMAKE_LINK_LIBRARY_FLAG "${CMAKE_LINK_LIBRARY_FLAG} \
    --start-group \
    --gc-sections \
    -T ../2_ips2/ips_sc4_pdo/SSTM32H753IITX_RAM.ld \
    -T ../2_ips2/ips_sc4_pdo/STM32H753IITX_FLASH.ld \
    -lm \
    -lnosys \
    -lgcc \
    -lc \
    -lstdc++ \
    --end-group
    ")

include_directories(
    ${patchSRC}/../Inc
    ${patchSRC}
    ${patchSRC}/../Startup
    )
set (SOURCES_C
    ${patchSRC}/main.c
    ${patchSRC}/stm32h7xx_hal_msp.c
    ${patchSRC}/stm32h7xx_it.c
    ${patchSRC}/syscalls.c
    ${patchSRC}/sysmem.c
    ${patchSRC}/system_stm32h7xx.c
    )

set (STARTUP_SOURCES ${patchSRC}/../Startup/startup_stm32h753iitx.s )
add_library(STARTUP ${STARTUP_SOURCES} )

include_directories( ${patchDRV}/BSP/Components/lan8742 )
set (BSP_SOURCES ${patchDRV}/BSP/Components/lan8742/lan8742.c )
add_library(BSP ${BSP_SOURCES} )

include_directories(
    ${patchDRV}/CMSIS/Include
    ${patchDRV}/CMSIS/Device/ST/STM32H7xx/Include
    )
#set (CMSYS_SOURCES
#    )
#add_library(CMSYS
#    )

include_directories(
    ${patchDRV}/STM32H7xx_HAL_Driver/Inc
    ${patchDRV}/STM32H7xx_HAL_Driver/Inc/Legacy
    )
file (GLOB srcHAL ${patchHAL_SRC}/*)
set (STM32H7xx_HAL_Driver_SOURCES ${srcHAL} )
add_library(STM32H7xx_HAL_Driver ${STM32H7xx_HAL_Driver_SOURCES})

include_directories(
    ../2_ips2/ips_sc4_pdo/LWIP/App
    ../2_ips2/ips_sc4_pdo/LWIP/Target
    )
set (LWIP_SOURCES
    ../2_ips2/ips_sc4_pdo/LWIP/App/lwip.c
    ../2_ips2/ips_sc4_pdo/LWIP/Target/ethernetif.c
    )
add_library(LWIP ${LWIP_SOURCES} )

add_library(Middlewares

    )


add_executable(${PROJECT_NAME}.elf
    ${SOURCES_C}
    )

set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINKER_LANGUAGE C)

target_link_libraries(${PROJECT_NAME}.elf PUBLIC
    STARTUP
    BSP
#    CMSIS
    STM32H7xx_HAL_Driver
    LWIP
    Middlewares
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
