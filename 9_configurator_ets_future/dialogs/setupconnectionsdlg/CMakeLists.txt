project(setupconnectionsdlg)
include(../../cmake/project.cmake)
message("-- Configuring "  ${PROJECT_NAME} " lib")

set(SETUPCONNECTIONSDLG_PATH ${APP_PATH}/dialogs/setupconnectionsdlg)
add_definitions(-DSETUPCONNECTIONS_DIALOG_LIBRARY)

include_directories(
    ${APP_PATH}
    ${SETUPCONNECTIONSDLG_PATH}
)
find_package(QT NAMES
    Qt5
    Qt6
    COMPONENTS
    Widgets
    Gui
    SerialPort
    Network
    REQUIRED
)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
    Widgets
    Gui
    SerialPort
    Network
    REQUIRED
)
set(HEADER_FILES
    ${SETUPCONNECTIONSDLG_PATH}/setupconnections.h
    ${SETUPCONNECTIONSDLG_PATH}/setupconnections_pch.h
    ${SETUPCONNECTIONSDLG_PATH}/setupconnections_global.h
)
set(SOURCE_FILES
    ${SETUPCONNECTIONSDLG_PATH}/setupconnections.cpp    
)
set(FORMS
    ${SETUPCONNECTIONSDLG_PATH}/setupconnections.ui
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
    Qt${QT_VERSION_MAJOR}::Network
)
if(MSVC)
    target_precompile_headers(${PROJECT_NAME}
        PRIVATE
        ${SETUPCONNECTIONSDLG_PATH}/setupconnections_pch.h
    )
endif()
