cmake_minimum_required(VERSION 3.14)

project(qtmulticast LANGUAGES CXX)

message(${CMAKE_SYSTEM_NAME})
if (WIN32)
    add_compile_definitions(__WIN32__)
    add_compile_definitions(_WINSOCK_DEPRECATED_NO_WARNINGS)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core REQUIRED)

add_compile_definitions(IPS_USING_QT)

set(MCAST_LIB_DIR .)

include_directories(${MCAST_LIB_DIR})

file(GLOB MCAST_LIB_SRC RELATIVE ${PROJECT_SOURCE_DIR} "${MCAST_LIB_DIR}/*.?pp")

add_library(qtmulticast SHARED ${MCAST_LIB_SRC})

if (WIN32)
    target_link_libraries(qtmulticast Ws2_32)
else()
    target_link_libraries(qtmulticast pthread)
endif()

target_link_libraries(qtmulticast Qt${QT_VERSION_MAJOR}::Core)

