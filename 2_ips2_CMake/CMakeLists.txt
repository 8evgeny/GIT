
cmake_minimum_required(VERSION 3.15)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Set ARM compiler
set(TOOLS_PATH /home/evg/toolchain/gcc-arm-none-eabi-new)
set(CMAKE_C_COMPILER ${TOOLS_PATH}/bin/arm-none-eabi-gcc)
#set(CMAKE_C_LINKER_LAUNCHER ${TOOLS_PATH}/bin/arm-none-eabi-g++)
#set(CMAKE_CXX_COMPIER ${TOOLS_PATH}/bin/arm-none-eabi-g++)

project(ips2 C CXX ASM)

#set(CMAKE_BUILD_TYPE debug)
#set(CMAKE_BUILD_TYPE release)

set (patchSRC "../2_ips2/ips_sc4_pdo/Core/Src")
set (patchDRV "../2_ips2/ips_sc4_pdo/Drivers")
set (patchHAL_SRC "${patchDRV}/STM32H7xx_HAL_Driver/Src")

#-mcpu=cortex-m7 \
#-mfloat-abi=hard \
#-mfpu=fpv5-sp-d16 \
#-mthumb \
#-Xlinker \
#--gc-sections \
#-specs=nosys.specs \
#-specs=nano.specs \


#    -c \
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}\
    -mcpu=cortex-m7 \
    -std=gnu11 \
    -DUSE_HAL_DRIVER \
    -DSTM32H753xx \
    -I../../2_ips2/ips_sc4_pdo/Core/Inc \
    -I../../2_ips2/ips_sc4_pdo/Core/Src \
    -I../../2_ips2/ips_sc4_pdo/Drivers/STM32H7xx_HAL_Driver/Inc \
    -I../../2_ips2/ips_sc4_pdo/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy \
    -I../../2_ips2/ips_sc4_pdo/Drivers/CMSIS/Device/ST/STM32H7xx/Include \
    -I../../2_ips2/ips_sc4_pdo/Drivers/CMSIS/Include -I../../2_ips2/ips_sc4_pdo/LWIP/ \
    -I../../2_ips2/ips_sc4_pdo/LWIP/App \
    -I../../2_ips2/ips_sc4_pdo/LWIP/Target \
    -I../../2_ips2/ips_sc4_pdo/Middlewares/Third_Party/LwIP/src/include \
    -I../../2_ips2/ips_sc4_pdo/Middlewares/Third_Party/LwIP/system \
    -I../../2_ips2/ips_sc4_pdo/Drivers/BSP/Components/lan8742 \
    -I../../2_ips2/ips_sc4_pdo/Drivers/BSP/Components/dp83848 \
    -I../../2_ips2/ips_sc4_pdo/Middlewares/Third_Party/LwIP/src/include/netif/ppp \
    -I../../2_ips2/ips_sc4_pdo/Middlewares/Third_Party/LwIP/src/include/lwip \
    -I../../2_ips2/ips_sc4_pdo/Middlewares/Third_Party/LwIP/src/include/lwip/apps \
    -I../../2_ips2/ips_sc4_pdo/Middlewares/Third_Party/LwIP/src/include/lwip/priv \
    -I../../2_ips2/ips_sc4_pdo/Middlewares/Third_Party/LwIP/src/include/lwip/prot \
    -I../../2_ips2/ips_sc4_pdo/Middlewares/Third_Party/LwIP/src/include/netif \
    -I../../2_ips2/ips_sc4_pdo/Middlewares/Third_Party/LwIP/src/include/compat/posix \
    -I../../2_ips2/ips_sc4_pdo/Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa \
    -I../../2_ips2/ips_sc4_pdo/Middlewares/Third_Party/LwIP/src/include/compat/posix/net \
    -I../../2_ips2/ips_sc4_pdo/Middlewares/Third_Party/LwIP/src/include/compat/posix/sys \
    -I../../2_ips2/ips_sc4_pdo/Middlewares/Third_Party/LwIP/src/include/compat/stdc \
    -I../../2_ips2/ips_sc4_pdo/Middlewares/Third_Party/LwIP/system/arch \
    -Os \
    -ffunction-sections \
    -fdata-sections \
    -Wall \
    -mno-unaligned-access \
    -fstack-usage \
    -MMD \
    -MP \
    --specs=nano.specs \
    -mfpu=fpv5-d16 \
    -mfloat-abi=hard \
    -mthumb \
    -T ${CMAKE_SOURCE_DIR}/../2_ips2/ips_sc4_pdo/STM32H753IITX_FLASH.ld \
    -T ${CMAKE_SOURCE_DIR}/../2_ips2/ips_sc4_pdo/STM32H753IITX_RAM.ld \
    ")

#    --gc-sections \
#    --end-group \

#--start-group \
#--data-sections \
#-T ${CMAKE_SOURCE_DIR}/../2_ips2/ips_sc4_pdo/STM32H753IITX_RAM.ld \
#-T ${CMAKE_SOURCE_DIR}/../2_ips2/ips_sc4_pdo/STM32H753IITX_FLASH.ld \
#-lm \
#-lnosys \
#-lgcc \
#-lc \
#-lstdc++ \

set(CMAKE_LINK_LIBRARY_FLAG "${CMAKE_LINK_LIBRARY_FLAG} \
    -mcpu=cortex-m7 \
    -T ${CMAKE_SOURCE_DIR}/../2_ips2/ips_sc4_pdo/STM32H753IITX_FLASH.ld \
    -T ${CMAKE_SOURCE_DIR}/../2_ips2/ips_sc4_pdo/STM32H753IITX_RAM.ld \
    --specs=nosys.specs \
    -Wl,-Map=ips_sc4_pdo.map \
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

include_directories(
    ${patchSRC}/../Inc
    ${patchSRC}
    ${patchSRC}/../Startup
    )
file (GLOB SOURCES_C ${patchSRC}/*.c )


include_directories(
    ${CMAKE_SOURCE_DIR}/../2_ips2/ips_sc4_pdo/Core/Src
    ${patchDRV}/CMSIS/Include
    ${patchDRV}/CMSIS/Device/ST/STM32H7xx/Include
    ${CMAKE_SOURCE_DIR}/../2_ips2/ips_sc4_pdo/Drivers/BSP/Components/dp83848
    )
file (GLOB CORE_SOURCES
    ${CMAKE_SOURCE_DIR}/../2_ips2/ips_sc4_pdo/Core/Src/*.c
    ${CMAKE_SOURCE_DIR}/../2_ips2/ips_sc4_pdo/Drivers/BSP/Components/dp83848/dp83848.c
    ${CMAKE_SOURCE_DIR}/../2_ips2/ips_sc4_pdo/LWIP/Target/ethernetif.c
    )
message (${CORE_SOURCES})

add_library(CORE ${CORE_SOURCES} )

set (STARTUP_SOURCES ${patchSRC}/../Startup/startup_stm32h753iitx.s )
add_library(STARTUP ${STARTUP_SOURCES} )

include_directories( ${patchDRV}/BSP/Components/lan8742 )
set (BSP_SOURCES ${patchDRV}/BSP/Components/lan8742/lan8742.c )
add_library(BSP ${BSP_SOURCES} )

#include_directories(
#    ${patchDRV}/CMSIS/Include
#    ${patchDRV}/CMSIS/Device/ST/STM32H7xx/Include
#    ../2_ips2/ips_sc4_pdo/Drivers/BSP/Components/dp83848
#    )

#set (CMSYS_SOURCES
#    ../2_ips2/ips_sc4_pdo/Drivers/BSP/Components/dp83848/dp83848.c
#    )
#add_library(CMSYS
#    ${CMSYS_SOURCES}
#    )

include_directories(
    ${patchDRV}/STM32H7xx_HAL_Driver/Inc
    ${patchDRV}/STM32H7xx_HAL_Driver/Inc/Legacy
    )
file (GLOB HAL_SRC ${patchHAL_SRC}/*)
add_library(STM32H7xx_HAL_Driver ${HAL_SRC})

include_directories(
    ${CMAKE_SOURCE_DIR}/../2_ips2/ips_sc4_pdo/LWIP/App
    ${CMAKE_SOURCE_DIR}/../2_ips2/ips_sc4_pdo/LWIP/Target
    ${CMAKE_SOURCE_DIR}/../2_ips2/ips_sc4_pdo/LWIP
    )
set (LWIP_SOURCES
    ${CMAKE_SOURCE_DIR}/../2_ips2/ips_sc4_pdo/LWIP/App/lwip.c
    ${CMAKE_SOURCE_DIR}/../2_ips2/ips_sc4_pdo/LWIP/Target/ethernetif.c
    ${CMAKE_SOURCE_DIR}/../2_ips2/ips_sc4_pdo/LWIP/ping.c
    )
add_library(LWIP ${LWIP_SOURCES} )

include_directories(
    ${CMAKE_SOURCE_DIR}/../2_ips2/ips_sc4_pdo/Middlewares/Third_Party/LwIP/system/arch
    ${CMAKE_SOURCE_DIR}/../2_ips2/ips_sc4_pdo/Middlewares/Third_Party/LwIP/system
    ${CMAKE_SOURCE_DIR}/../2_ips2/ips_sc4_pdo/Middlewares/Third_Party/LwIP/src/include/netif
    ${CMAKE_SOURCE_DIR}/../2_ips2/ips_sc4_pdo/Middlewares/Third_Party/LwIP/src/include/netif/ppp
    ${CMAKE_SOURCE_DIR}/../2_ips2/ips_sc4_pdo/Middlewares/Third_Party/LwIP/src/include
    ${CMAKE_SOURCE_DIR}/../2_ips2/ips_sc4_pdo/Middlewares/Third_Party/LwIP/src/include/lwip
    ${CMAKE_SOURCE_DIR}/../2_ips2/ips_sc4_pdo/Middlewares/Third_Party/LwIP/src/include/lwip/apps
    ${CMAKE_SOURCE_DIR}/../2_ips2/ips_sc4_pdo/Middlewares/Third_Party/LwIP/src/include/lwip/priv
    ${CMAKE_SOURCE_DIR}/../2_ips2/ips_sc4_pdo/Middlewares/Third_Party/LwIP/src/include/lwip/prot
    ${CMAKE_SOURCE_DIR}/../2_ips2/ips_sc4_pdo/Middlewares/Third_Party/LwIP/src/include/compat/posix
    ${CMAKE_SOURCE_DIR}/../2_ips2/ips_sc4_pdo/Middlewares/Third_Party/LwIP/src/include/compat/arpa
    ${CMAKE_SOURCE_DIR}/../2_ips2/ips_sc4_pdo/Middlewares/Third_Party/LwIP/src/include/compat/net
    ${CMAKE_SOURCE_DIR}/../2_ips2/ips_sc4_pdo/Middlewares/Third_Party/LwIP/src/include/compat/sys
    )

file (GLOB Third_Party_SRC
    ${CMAKE_SOURCE_DIR}/../2_ips2/ips_sc4_pdo/Middlewares/Third_Party/LwIP/src/api/*
    ${CMAKE_SOURCE_DIR}/../2_ips2/ips_sc4_pdo/Middlewares/Third_Party/LwIP/src/apps/mqtt/mqtt.c
    ${CMAKE_SOURCE_DIR}/../2_ips2/ips_sc4_pdo/Middlewares/Third_Party/LwIP/src/core/*
    ${CMAKE_SOURCE_DIR}/../2_ips2/ips_sc4_pdo/Middlewares/Third_Party/LwIP/src/core/ipv4/*
    ${CMAKE_SOURCE_DIR}/../2_ips2/ips_sc4_pdo/Middlewares/Third_Party/LwIP/src/netif/*
    ${CMAKE_SOURCE_DIR}/../2_ips2/ips_sc4_pdo/Middlewares/Third_Party/LwIP/src/netif/ppp/*
    )

add_library(Middlewares
    ${Third_Party_SRC}
    )

add_executable(${PROJECT_NAME}.elf
    ${SOURCES_C}
    )

set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINKER_LANGUAGE C)

target_link_libraries(${PROJECT_NAME}.elf PUBLIC
    CORE
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
