cmake_minimum_required(VERSION 3.14)

project(protocols LANGUAGES CXX)

set(ISLP_BUILD_EXAMPLES ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5Core)

add_compile_definitions(IPS_USING_QT)

set(PROTO_LIB_DIR islp)
set(TOOLS_LIB_DIR tools)
set(NAMES_LIB_DIR names)

include_directories(${PROTO_LIB_DIR})
include_directories(${TOOLS_LIB_DIR})
include_directories(${NAMES_LIB_DIR})

file(GLOB PROTO_LIB_SRC RELATIVE ${PROJECT_SOURCE_DIR} "${PROTO_LIB_DIR}/*.?pp" )
file(GLOB TOOLS_LIB_SRC RELATIVE ${PROJECT_SOURCE_DIR} "${TOOLS_LIB_DIR}/*.?pp" )
file(GLOB NAMES_LIB_SRC RELATIVE ${PROJECT_SOURCE_DIR} "${NAMES_LIB_DIR}/*.?pp" )

if(ISLP_BUILD_EXAMPLES)
    add_subdirectory(usage_example_qt/multicaster/ineter ${PROJECT_BINARY_DIR}/multicaster)
    add_subdirectory(usage_example_qt/server ${PROJECT_BINARY_DIR}/server)
    add_subdirectory(usage_example_qt/client ${PROJECT_BINARY_DIR}/client)
endif()


add_library(libislp STATIC
    ${PROTO_LIB_SRC}
    ${TOOLS_LIB_SRC}
    ${NAMES_LIB_SRC}
)

target_link_libraries(libislp Qt${QT_VERSION_MAJOR}::Core)
