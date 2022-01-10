cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_STANDARD 17)
project(display)

include(${CMAKE_SOURCE_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(${PROJECT_NAME}
    src/main.cpp
    )

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
    )
include_directories(
    inc/
    )

target_link_libraries(${PROJECT_NAME}
    ${CONAN_LIBS}
    )
