cmake_minimum_required(VERSION 3.5)

set (name CONFIGURATOR)
project(${name} LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file (GLOB src
    Lib/qjson/src/cmdline_tester.cpp
    Lib/qjson/src/cmdlineparser.cpp
    Lib/qjson/src/json_parser.cc
    Lib/qjson/src/json_scanner.cc
    Lib/qjson/src/json_scanner.cpp
    Lib/qjson/src/parser.cpp
    Lib/qjson/src/parserrunnable.cpp
    Lib/qjson/src/qobjecthelper.cpp
    Lib/qjson/src/serializer.cpp
    Lib/qjson/src/serializerrunnable.cpp
    Lib/aes/qaesencryption.cpp
    appcore.cpp
    assignment_list.cpp
    firmware.cpp
    groups.cpp
    main.cpp
    network.cpp
    programmer.cpp
    receiver.cpp
    subscriber.cpp
    terminal.cpp
    verification.cpp
    )

find_package (Qt5 REQUIRED COMPONENTS
    Core
    Widgets
    Quick
    Gui
    Network
    Sql
    SerialPort
    Test
    )

add_executable(${PROJECT_NAME} ${src})

set_target_properties(${PROJECT_NAME}  PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
    )

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Widgets
    Qt5::Quick
    Qt5::Sql
    Qt5::Gui
    Qt5::Network
    Qt5::SerialPort
    Qt5::Test
    )

include_directories(${PROJECT_NAME}
    Lib/aes
    lib/qjson/include/
    )
