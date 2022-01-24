cmake_minimum_required(VERSION 3.14)

project(client LANGUAGES CXX)

message(${CMAKE_SYSTEM_NAME})
if (WIN32)
    message ("Windows")
    add_compile_definitions(__WIN32__)
    add_compile_definitions(_WINSOCK_DEPRECATED_NO_WARNINGS)
#   set(CMAKE_PREFIX_PATH "D:\\Qt\\5.15.2\\msvc2019_64\\")
elseif(UNIX)
    message ("POSIX")
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

add_compile_definitions(IPS_USING_QT)

set(PROTO_LIB_DIR ../../islp)
set(TOOLS_LIB_DIR ../../tools)
set(NAMES_LIB_DIR ../../names)
set(ISLP_LIB_DIR ../..)
set(MCAST_LIB_DIR ../multicaster/ineter)
set(PROCESS_LIB_DIR tools)

include_directories(${PROTO_LIB_DIR})
include_directories(${TOOLS_LIB_DIR})
include_directories(${NAMES_LIB_DIR})
include_directories(${ISLP_LIB_DIR})
include_directories(${MCAST_LIB_DIR})
include_directories(${PROCESS_LIB_DIR})


file(GLOB PROCESS_LIB_SRC RELATIVE ${PROJECT_SOURCE_DIR} "${PROCESS_LIB_DIR}/*.?pp" )


if(ISLP_BUILD_EXAMPLES)
else()
    add_subdirectory(../multicaster/ineter ${PROJECT_BINARY_DIR}/mcast_lib)
    add_subdirectory(../.. ${PROJECT_BINARY_DIR}/libislp)
endif()

add_executable(client
  main.cpp
  clientwindow.cpp
  clientwindow.h
  clientwindow.ui
  ${PROCESS_LIB_SRC}
)

if (WIN32)
else()
    target_link_libraries(client pthread)
endif()

target_link_libraries(client qtmulticast)
target_link_libraries(client libislp)
target_link_libraries(client Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(client Qt${QT_VERSION_MAJOR}::Gui)
target_link_libraries(client Qt${QT_VERSION_MAJOR}::Widgets)


