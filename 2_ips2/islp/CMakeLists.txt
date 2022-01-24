cmake_minimum_required(VERSION 3.14)

project(protocols LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
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

add_executable(protocols
  main.cpp
  ${PROTO_LIB_SRC}
  ${TOOLS_LIB_SRC}
  ${NAMES_LIB_SRC}
)
target_link_libraries(protocols Qt${QT_VERSION_MAJOR}::Core)
