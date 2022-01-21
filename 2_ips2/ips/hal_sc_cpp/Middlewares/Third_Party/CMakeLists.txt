cmake_minimum_required(VERSION 3.14)

project(lwip LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)



set(LIBS_BUILD ON)
set(TESTS ON)
set(TARGET_BUILD ON)


if(TARGET_BUILD)

else()
    message(STATUS "Platform test build")
    add_compile_definitions(PLATFORM_TEST)
    add_compile_definitions(PLATFORM_TEST_PERF)
endif()

set(IP_DIR LwIP)
set(IP_ARCH_HDRS LwIP/system/arch)
include_directories(${IP_ARCH_HDRS})
file(GLOB_RECURSE LWIP_SRC  RELATIVE ${PROJECT_SOURCE_DIR}/${IP_DIR} "*.h*" "*.c*")
message(STATUS "Source files:${LWIP_SRC}")

#target_compile_definitions(
#  ${EXECUTABLE}
#  PRIVATE -DDEBUG
#          -DUSE_FULL_ASSERT
#          -DSTM32F407xx
#          -DTRACE
#          -DOS_USE_TRACE_SEMIHOSTING_DEBUG
#          -DOS_USE_TRACE_SEMIHOSTING_STDOUT
#          -DOS_USE_SEMIHOSTING)

## only set for debug at the moment
target_compile_options(
  lwip
          -mcpu=cortex-m7
          -mthumb
          -mfpu=fpv5-sp-d16
          -mfloat-abi=hard
          -fdata-sections
          -ffunction-sections
          -Wall
          )

target_link_options(
  lwip
  -T${PROJECT_SOURCE_DIR}/../../STM32H753IITX_FLASH.ld
  -mcpu=cortex-m7
  -mthumb
  -mfpu=fpv5-sp-d16
  -mfloat-abi=hard
  -nostartfiles
  -specs=nano.specs
  -specs=rdimon.specs
  -Wl,--build-id
  -lc
  -lm
  # -lnosys
  -Wl,-Map=${PROJECT_NAME}.map,--cref
  -Wl,--gc-sections)


add_library(lwip STATIC ${LWIP_SRC})









    
