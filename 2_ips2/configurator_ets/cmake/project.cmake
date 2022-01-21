if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
#    set(GUI_TYPE WIN32)
    find_library(Wsock32_LIBRARY wsock32)
    add_compile_definitions(__WIN32__)
    add_compile_definitions(_WINSOCK_DEPRECATED_NO_WARNINGS)
    set(SOCKS_LIBS wsock32 ws2_32)
endif()

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(QT_LUPDATE_EXECUTABLE  ${CMAKE_PREFIX_PATH}/bin/lupdate)
set(QT_LRELEASE_EXECUTABLE ${CMAKE_PREFIX_PATH}/bin/lrelease)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY AUTOGEN_TARGETS_FOLDER AutoMoc)

add_definitions(-DQT)
add_definitions(-DQT_NO_CAST_FROM_ASCII)
add_definitions(-DQT_NO_CAST_TO_ASCII)
add_definitions(-DQT_NO_CAST_FROM_BYTEARRAY)
add_definitions(-DQT_NO_URL_CAST_FROM_STRING)
add_definitions(-DQT_USE_QSTRINGBUILDER)
add_definitions(-D_SCL_SECURE_NO_WARNINGS)

set(RESOURCES
    ${RESOURCES_PATH}/images/svg.qrc
    ${RESOURCES_PATH}/css/css.qrc
)
set(TranslationFiles
   ${TRANSLATIONS_PATH}/resources/langs/en.ts
   ${TRANSLATIONS_PATH}/resources/langs/ru.ts
)
