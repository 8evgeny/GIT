cmake_minimum_required(VERSION 3.14)

project(ufwLibs LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(UFW_CXX_CHECK OFF)

set(LIBS_BUILD ON)
set(TESTS OFF)
set(TARGET_BUILD OFF)
set(TEST_DEV OFF)
set(TEST_CONTAINERS OFF)
set(TEST_CONTAINERS OFF)

if(UFW_CXX_CHECK)
    add_compile_definitions(UFW_CXX_CHECK)
endif()

if(TARGET_BUILD)

else()
    message(STATUS "Platform test build")
    add_compile_definitions(PLATFORM_TEST)
    add_compile_definitions(PLATFORM_TEST_PERF)
endif()

set(U_LIB_DIR libs)
set(U_ALLOCATOR_DIR ${U_LIB_DIR}/allocation)
set(U_HEAP_DIR ${U_ALLOCATOR_DIR}/heap)
set(U_CONTAINERS_DIR ${U_LIB_DIR}/containers)
set(U_DEVICES_DIR ${U_LIB_DIR}/devices)
set(U_MEMORY_DIR ${U_LIB_DIR}/memory)
set(U_OS_DIR ${U_LIB_DIR}/os)
set(U_SYSTEM_DIR ${U_LIB_DIR}/system)
set(U_ERRORS_DIR ${U_LIB_DIR}/error)
set(U_TESTS_DIR ${U_LIB_DIR}/tests)
set(U_COMMON_DIR ${U_LIB_DIR}/common)

include_directories(${U_LIB_DIR})
include_directories(${U_ALLOCATOR_DIR})
include_directories(${U_HEAP_DIR})
include_directories(${U_CONTAINERS_DIR})
include_directories(${U_DEVICES_DIR})
include_directories(${U_MEMORY_DIR})
include_directories(${U_OS_DIR})
include_directories(${U_SYSTEM_DIR})
include_directories(${U_ERRORS_DIR})
include_directories(${U_TESTS_DIR})

#find_file(U_LIB_SOURCE  "*hpp"  ${U_CONTAINERS_DIR})
file(GLOB U_LIB_SRC  RELATIVE ${PROJECT_SOURCE_DIR} "${U_LIB_DIR}/*.h" "${U_LIB_DIR}/*.hpp")
file(GLOB U_ALLOCATOR_SRC RELATIVE ${PROJECT_SOURCE_DIR} "${U_ALLOCATOR_DIR}/*.?pp" "${U_ALLOCATOR_DIR}/*.h")
file(GLOB U_HEAP_SRC RELATIVE ${PROJECT_SOURCE_DIR} "${U_HEAP_DIR}/*.?pp" "${U_HEAP_DIR}/*.h")
file(GLOB U_CONTAINERS_SRC RELATIVE ${PROJECT_SOURCE_DIR} "${U_CONTAINERS_DIR}/*.?pp" "${U_CONTAINERS_DIR}/*.h" )
file(GLOB U_DEVICES_SRC RELATIVE ${PROJECT_SOURCE_DIR} "${U_DEVICES_DIR}/*.?pp" "${U_DEVICES_DIR}/*.h" "${U_DEVICES_DIR}/*/*.?pp" "${U_DEVICES_DIR}/*/*.h")
file(GLOB U_MEMORY_SRC RELATIVE ${PROJECT_SOURCE_DIR} "${U_MEMORY_DIR}/*.?pp" "${U_MEMORY_DIR}/*.h")
file(GLOB U_OS_SRC RELATIVE ${PROJECT_SOURCE_DIR} "${U_OS_DIR}/*.?pp" "${U_OS_DIR}/*.h")
file(GLOB U_SYSTEM_SRC RELATIVE ${PROJECT_SOURCE_DIR} "${U_SYSTEM_DIR}/*.?pp" "${U_SYSTEM_DIR}/*.h" "${U_ERRORS_DIR}/*.?pp" "${U_ERRORS_DIR}/*.h")
file(GLOB U_COMMON_SRC RELATIVE ${PROJECT_SOURCE_DIR} "${U_COMMON_DIR}/*.?pp" "${U_COMMON_DIR}/*.h")

file(GLOB U_TESTS_SRC RELATIVE ${PROJECT_SOURCE_DIR} "${U_TESTS_DIR}/*.?pp" "${U_TESTS_DIR}/*.h")
file(GLOB U_BENCHMARK_SRC RELATIVE ${PROJECT_SOURCE_DIR} "${U_TESTS_DIR}/benchmark*.*")

message(STATUS "Source files:${U_LIB_SRC}")
message(STATUS "Source files:${U_ALLOCATOR_SRC}")
message(STATUS "Source files:${U_HEAP_SRC}")
message(STATUS "Source files:${U_CONTAINERS_SRC}")
message(STATUS "Source files:${U_DEVICES_SRC}")
message(STATUS "Source files:${U_MEMORY_SRC}")
message(STATUS "Source files:${U_OS_SRC}")
message(STATUS "Source files:${U_SYSTEM_SRC}")
message(STATUS "Source files:${U_COMMON_DIR}")
message(STATUS "Source files:${U_TESTS_SRC}")

if(LIBS_BUILD)
    add_subdirectory(libips)
    add_library(ufwLibs STATIC
        ${U_LIB_SRC}
        ${U_ALLOCATOR_SRC}
        ${U_HEAP_SRC}
        ${U_CONTAINERS_SRC}
        ${U_DEVICES_SRC}
        ${U_MEMORY_SRC}
        ${U_OS_SRC}
        ${U_SYSTEM_SRC}
        ${U_COMMON_SRC}
#        ${U_ERRORS_SRC}
    #    ${U_TESTS_SRC}
        )
endif()

if(TESTS)
    if(TEST_DEV)
        list(APPEND U_TEST_D_SRC ${U_DEVICES_SRC})
    #    list(APPEND U_TEST_SRC ${U_TESTS_DIR}/device_tests.cpp)
    endif()
    if(TEST_CONTAINERS)
        list(APPEND U_TESTS_C_SRC ${U_CONTAINERS_SRC})
    #    list(APPEND U_TESTS_SRC ${U_TESTS_DIR}/containers_test.cpp)
    endif()



    add_executable(
        heap_test
        ${U_TESTS_DIR}/tests.cpp
        ${U_TESTS_SRC}
        ${U_BENCHMARK_SRC}
        ${U_ALLOCATOR_SRC}
        ${U_HEAP_SRC}
        ${U_SYSTEM_SRC}
        ${U_COMMON_SRC}
        ${U_TESTS_C_SRC}
        ${U_TEST_D_SRC}
        )
endif()

#add_executable(ufwexec exec.cpp
#        libs.cpp
#    ${U_LIB_SRC}
#    ${U_ALLOCATOR_SRC}
#    ${U_HEAP_SRC}
#    ${U_CONTAINERS_SRC}
#    ${U_DEVICES_SRC}
#    ${U_MEMORY_SRC}
#    ${U_OS_SRC}
#    ${U_SYSTEM_SRC}
#    ${U_ERRORS_SRC}
##    ${U_TESTS_SRC}
#    )
    
