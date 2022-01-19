cmake_minimum_required(VERSION 3.5)

project(ips_aes LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories ("${PROJECT_SOURCE_DIR}/inc")


add_library(ips_aes src/AES.cpp)
