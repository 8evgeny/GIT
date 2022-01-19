cmake_minimum_required(VERSION 3.14)

project(libips LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIBS_BUILD ON)

set(TARGET_BUILD OFF)
set(TEST_DEV OFF)


if(TARGET_BUILD)

else()
    message(STATUS "Platform test build")
    add_compile_definitions(PLATFORM_TEST)
    add_compile_definitions(PLATFORM_TEST_PERF)
endif()

set(LI_LIB_DIR ${PROJECT_SOURCE_DIR})

include_directories(${LI_LIB_DIR})


#find_file(U_LIB_SOURCE  "*hpp"  ${U_CONTAINERS_DIR})
file(GLOB_RECURSE LI_LIB_SRC  RELATIVE ${PROJECT_SOURCE_DIR} "*.h" "*.c*")

message(STATUS "Source files:${LI_LIB_SRC}")

if(LIBS_BUILD)
    add_library(
        libips 
        STATIC
        ${LI_LIB_SRC}
        )
endif()
