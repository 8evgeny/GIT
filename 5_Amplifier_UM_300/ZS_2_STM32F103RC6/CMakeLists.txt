SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.7)

# specify cross compilers and tools
SET(CMAKE_C_COMPILER_WORKS 1)
SET(CMAKE_C_COMPILER arm-none-eabi-gcc)
SET(CMAKE_CXX_COMPILER_WORKS 1)
SET(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER  arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)

#SET(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F103C8TX_FLASH.ld)
#SET(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/stm32f103c8t6.ld)
SET(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/stm32f103rct6.ld)

#Uncomment for hardware floating point
#SET(FPU_FLAGS "-mfloat-abi=hard -mfpu=fpv4-sp-d16")
#add_definitions(-DARM_MATH_CM4 -DARM_MATH_MATRIX_CHECK -DARM_MATH_ROUNDING -D__FPU_PRESENT=1)
add_definitions(-DUSE_STDPERIPH_DRIVER -DUSE_FULL_ASSERT)

#Uncomment for software floating point
#SET(FPU_FLAGS "-mfloat-abi=soft")

SET(COMMON_FLAGS
    "-mcpu=cortex-m3 \
    ${FPU_FLAGS} \
    -mthumb \
    -mthumb-interwork \
    -ffunction-sections \
    -fdata-sections \
    -g \
    -fno-common \
    -fmessage-length=0 \
    ")

SET(CMAKE_CXX_FLAGS_INIT "${COMMON_FLAGS} -std=c++11")
SET(CMAKE_C_FLAGS_INIT "${COMMON_FLAGS} -std=gnu99")

SET(CMAKE_EXE_LINKER_FLAGS_INIT " \
    -Wl,-gc-sections,--print-memory-usage \
    -T ${LINKER_SCRIPT} \
    ")

PROJECT(5_stm32f103rc6 C CXX ASM)
set(CMAKE_CXX_STANDARD 11)

set(I2C_version 2)

#add_definitions(-DARM_MATH_CM4 -DARM_MATH_MATRIX_CHECK -DARM_MATH_ROUNDING -D__FPU_PRESENT=1)
add_definitions(-DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB
    -DSTM32F1 #добавил
    )

file(GLOB_RECURSE SOURCES_C
    src/*.c
    rtos/heap_4.c
    rtos/list.c
    rtos/port.c
    rtos/tasks.c
    rtos/opencm3.c
    rtos/queue.c
    libwwg/src/uartlib.c
    )
if(${I2C_version} EQUAL 1)

file(GLOB_RECURSE SOURCES_I2C
    i2c_vers_1/i2c.c
    i2c_vers_1/i2c_main.c
    libwwg/src/usbcdc.c
    )
include_directories(
    i2c_vers_1
    )
endif()

if(${I2C_version} EQUAL 2)

file(GLOB_RECURSE SOURCES_I2C
    i2c_vers_2/cmsis_boot/startup/startup_stm32f10x_md.S
    i2c_vers_2/cmsis_boot/system_stm32f10x.c
    i2c_vers_2/src/*.*
    i2c_vers_2/stm_lib/src/*.*
    )
include_directories(
    i2c_vers_2/inc
    i2c_vers_2/cmsis
    i2c_vers_2/cmsis_boot
    i2c_vers_2/stm_lib/inc
    )
endif()


file(GLOB_RECURSE SOURCES_CPP
    src/*.cpp
    src_classes/*.cpp
    )


include_directories(
    inc
    libopencm3/include
    rtos
    ./
    libwwg/include
    src_classes
    )



add_executable(${PROJECT_NAME}.elf
    ${SOURCES_C}
    ${SOURCES_I2C}
    ${SOURCES_CPP}
    ${LINKER_SCRIPT}
    )

target_link_libraries(${PROJECT_NAME}.elf
    --static -nostartfiles
    -specs=nosys.specs
    -Wl,--start-group
    -lc
    -lgcc
    -lnosys
    -Wl,--end-group
    -L${CMAKE_SOURCE_DIR}/libopencm3/lib -lopencm3_stm32f1
    -L${CMAKE_SOURCE_DIR}/libwwg -lwwg
    )


#set(CMAKE_EXE_LINKER_FLAGS
#    "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map")

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")
