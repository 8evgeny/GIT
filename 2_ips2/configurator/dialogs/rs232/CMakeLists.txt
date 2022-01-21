project(rs232dlg)
include(../../cmake/project.cmake)
message("-- Configuring "  ${PROJECT_NAME} " lib")

set(RS232_PATH ${APP_PATH}/dialogs/rs232)
add_definitions(-DRS232_DIALOG_LIBRARY)

include_directories(
    ${APP_PATH}
    ${RS232_PATH}
)
find_package(QT NAMES
    Qt5
    Qt6
    COMPONENTS
    Widgets
    Gui
    SerialPort
    REQUIRED
)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
    Widgets
    Gui
    SerialPort
    REQUIRED
)
set(HEADER_FILES
    ${RS232_PATH}/rs232.h
    ${RS232_PATH}/rs232_pch.h
    ${RS232_PATH}/rs232_global.h
)
set(SOURCE_FILES
    ${RS232_PATH}/rs232.cpp    
)
set(FORMS
    ${RS232_PATH}/rs232.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_wrap_ui(UI_FILES
        ${FORMS}
    )
else()
    qt5_wrap_ui(UI_FILES
        ${FORMS}
    )
endif()

add_library(${PROJECT_NAME} SHARED
    ${HEADER_FILES}
    ${SOURCE_FILES}
    ${UI_FILES}
    ${RESOURCES_FILES}
    ${FORMS}
)
target_link_libraries(${PROJECT_NAME}
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::SerialPort
)
if(MSVC)
    target_precompile_headers(${PROJECT_NAME}
        PRIVATE
        ${RS232_PATH}/rs232_pch.h
    )
endif()
