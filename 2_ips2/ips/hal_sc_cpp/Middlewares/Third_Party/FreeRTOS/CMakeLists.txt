cmake_minimum_required(VERSION 3.14)

project(freertos LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
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

set(OS_DIR Source)

include_directories(${OS_DIR}/include)
file(GLOB_RECURSE OS_SRC  RELATIVE ${PROJECT_SOURCE_DIR}/${OS_DIR} "*.h*" "*.c*")
message(STATUS "Source files:${OS_SRC}")

add_library(freertos.o STATIC ${OS_SRC})









    
