cmake_minimum_required(VERSION 3.14)

project(json_parser LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra")
#;-finput-charset=UTF8")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -pthread -fcoroutines")

include_directories(rapidjson/include)

find_package(QT NAMES Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

add_executable(json_parser
  main.cpp
)
target_link_libraries(json_parser
    Qt${QT_VERSION_MAJOR}::Core
    )
add_subdirectory(rapidjson)
