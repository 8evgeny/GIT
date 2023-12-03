cmake_minimum_required(VERSION 3.5)

project(ATmega128 LANGUAGES C)

add_executable(ATmega128 main.c)
add_definitions(-D__AVR_ATmega128__)
install(TARGETS ATmega128
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
