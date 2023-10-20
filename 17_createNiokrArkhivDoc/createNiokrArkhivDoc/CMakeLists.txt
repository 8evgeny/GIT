cmake_minimum_required(VERSION 3.14)

project(createNiokrArkhivDoc LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra")
#;-finput-charset=UTF8")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -pthread -fcoroutines")

include_directories(rapidjson/include src)
find_package(QT NAMES Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
find_package (Boost REQUIRED COMPONENTS
    system
    filesystem
    program_options
    )
file(GLOB src src/*.cpp)
add_executable(${PROJECT_NAME} ${src})
target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Core -lncursesw ${Boost_LIBRARIES})
add_subdirectory(rapidjson)

