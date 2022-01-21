cmake_minimum_required(VERSION 3.14)


project(gitcomm LANGUAGES ASM C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIBS_BUILD ON)
set(TESTS OFF)
set(TARGET_BUILD ON)

#define mcu model
add_compile_definitions(STM32H753xx)




SET(STARTUP_PATH ${PROJECT_SOURCE_DIR}/Core/Startup/)
SET(STARTUP_CODE "${STARTUP_PATH}/startup_stm32h753iitx.s")
message(STATUS ":${STARTUP_CODE}")
file(GLOB_RECURSE _IPS_SRC_CODE RELATIVE ${PROJECT_SOURCE_DIR} "*.c" "*.?pp" "*.h")
message(STATUS "Source files:${_IPS_SRC_CODE}")
SET(LINKER_SCRIPT "${PROJECT_SOURCE_DIR}/STM32H753IITX_FLASH.ld")
#Toolchain setup section

#set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

#SET(TARGET_TRIPLET "arm-none-eabi")
##SET(TOOLCHAIN_BIN_DIR "/mnt/tools/toolchains/arm/gcc-arm-none-eabi-9-2020-q2-update/bin")
#SET(TOOLCHAIN_BIN_DIR "/usr/bin")


#SET(CMAKE_C_COMPILER "${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-gcc${TOOL_EXECUTABLE_SUFFIX}")
#SET(CMAKE_CXX_COMPILER "${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-g++${TOOL_EXECUTABLE_SUFFIX}")
#SET(CMAKE_ASM_COMPILER "${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-gcc${TOOL_EXECUTABLE_SUFFIX}")

SET(CMAKE_OBJCOPY "${TOOLCHAIN_BIN_DIR}/${TARGETSTATIC_TRIPLET}-objcopy${TOOL_EXECUTABLE_SUFFIX}" CACHE INTERNAL "objcopy tool")
SET(CMAKE_OBJDUMP "${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-objdump${TOOL_EXECUTABLE_SUFFIX}" CACHE INTERNAL "objdump tool")
SET(CMAKE_SIZE "${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-size${TOOL_EXECUTABLE_SUFFIX}" CACHE INTERNAL "size tool")
SET(CMAKE_DEBUGER "${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-gdb${TOOL_EXECUTABLE_SUFFIX}" CACHE INTERNAL "debuger")
SET(CMAKE_CPPFILT "${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-c++filt${TOOL_EXECUTABLE_SUFFIX}" CACHE INTERNAL "C++filt")


SET(CMAKE_BUILD_TYPE Release)
#Add your own GCC compile flags (set CPU core type (Cortex M3), set CPU instruction set mode (Thumb2)
SET(GCC_COVERAGE_COMPILE_FLAGS "-mcpu=cortex-m7 -std=gnu11 -g3  -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb")
SET(GXX_COVERAGE_COMPILE_FLAGS "-mcpu=cortex-m7 -std=gnu++14 -g3 -ffunction-sections -fdata-sections -fno-exceptions -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb")
#Add your own linker flags
SET(GCC_COVERAGE_LINK_FLAGS "-mcpu=cortex-m7 -T${LINKER_SCRIPT} --specs=nosys.specs -Wl,--cref -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group" )
#Override system C
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
#C++ compiler flags
SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
#Override system linker flags
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")





include_directories(${PROJECT_SOURCE_DIR}/Core/Inc/)
include_directories(${PROJECT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Inc/)
include_directories(${PROJECT_SOURCE_DIR}/Drivers/CMSIS/Include/)
include_directories(${PROJECT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Inc/)
include_directories(${PROJECT_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32H7xx/Include/)



SET(LINKER_SCRIPT "${PROJECT_SOURCE_DIR}/STM32H753IITX_FLASH.ld")


if(TARGET_BUILD)
    message(STATUS "Platform target must built")
else()
    message(STATUS "Platform test build")
    add_compile_definitions(PLATFORM_TEST)
    add_compile_definitions(PLATFORM_TEST_PERF)
endif()






message(STATUS "Source files:${_IPS_SRC_CODE}")
add_executable(
    gitcomm
    ${PROJECT_SOURCE_DIR}/Core/Src/syscalls.c
    ${STARTUP_PATH}/startup_stm32h753iitx.s
    ${_IPS_SRC_CODE}
    )


