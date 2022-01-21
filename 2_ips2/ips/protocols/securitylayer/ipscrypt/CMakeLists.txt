cmake_minimum_required(VERSION 3.5)

project(ipscrypt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(IPS_LINUX)

include_directories ("${PROJECT_SOURCE_DIR}/inc")
include_directories ("${PROJECT_SOURCE_DIR}AES/inc")
include_directories ("${PROJECT_SOURCE_DIR}MD5/inc")

add_subdirectory(AES)
add_subdirectory(MD5)

add_library(ipscrypt src/ipskeygen.cpp src/cryptkey.cpp src/ipscryptographic.cpp)
