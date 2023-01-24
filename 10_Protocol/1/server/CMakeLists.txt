cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_STANDARD 20)
project(server)

#include(${CMAKE_SOURCE_DIR}/conanbuildinfo.cmake)
#conan_basic_setup()

file (GLOB SRC src/*.cpp SRC src/*.c)

add_executable(${PROJECT_NAME}
    ${SRC}
    )

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
    )
include_directories(
    inc
    )

target_link_libraries(${PROJECT_NAME}
#    ${CONAN_LIBS}
    pthread
    )
