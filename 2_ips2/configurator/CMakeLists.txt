cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(IPSConfigurator)
include(cmake/version.cmake)

set(COMPANY "GIT")
set(COPYRIGHT "Copyright (c) 2018-2021 GIT. All rights reserved.")
set(IDENTIFIER "git-holding.ru")
set(QT_MINIMUM_VERSION 5.15.2)

set(INSTALL_PATH "bin/git/ips/configurator")
set(APP_PATH ${CMAKE_CURRENT_LIST_DIR})
set(OUTPUT_PATH ${APP_PATH}/bin)
set(RESOURCES_PATH ${APP_PATH}/resources)
set(DISTRIBS_PATH ${APP_PATH}/resources/distrib)
set(TRANSLATIONS_PATH ${APP_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_PATH})

include(cmake/flags.cmake)
include(cmake/deb.cmake)
include(cmake/install.cmake)
include(cmake/project.cmake)
include(CPack)

if(CMAKE_BUILD_TYPE STREQUAL Release)
    message("----- Release mode -----")
else()
    message("----- Debug mode -----")
    add_definitions(-DSURV_DEBUG)
    add_definitions(-DSURV_TESTDATA)
endif()

message("-- Project               " ${PROJECT_NAME})
message("-- Qt5 path:             " ${CMAKE_PREFIX_PATH})
message("-- Qt5 lupdate:          " ${QT_LUPDATE_EXECUTABLE})
message("-- Qt5 lrelease path:    " ${QT_LRELEASE_EXECUTABLE})
message("-- Configurator path:    " ${APP_PATH})
message("-- Resources path:       " ${RESOURCES_PATH})
message("-- Source path:          " ${CMAKE_SOURCE_DIR})
message("-- Binary path:          " ${CMAKE_BINARY_DIR})
message("-- Input path:          " ${OUTPUT_PATH})
message("-- Qt5 minimum version:  " ${QT_MINIMUM_VERSION})

include(build/conanbuildinfo.cmake)
conan_basic_setup(NO_OUTPUT_DIRS TARGETS)

add_subdirectory(dialogs)
add_subdirectory(main)

message("------------------")

INSTALL(TARGETS ips-configurator
        RUNTIME DESTINATION ${INSTALL_PATH})
