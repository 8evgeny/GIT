project(communication)
include(../../cmake/project.cmake)
message("-- Configuring "  ${PROJECT_NAME} " lib")

set(COMMUNICATION_PATH ${APP_PATH}/dialogs/communication)
add_definitions(-DCOMMUNICATION_DIALOG_LIBRARY)

include_directories(
    ${APP_PATH}
    ${COMMUNICATION_PATH}
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
    ${COMMUNICATION_PATH}/communication.h
    ${COMMUNICATION_PATH}/communication_pch.h
    ${COMMUNICATION_PATH}/communication_global.h
)
set(SOURCE_FILES
    ${COMMUNICATION_PATH}/communication.cpp    
)
set(LIBS
	cryptodlg
) 
add_library(${PROJECT_NAME} SHARED
    ${HEADER_FILES}
    ${SOURCE_FILES}
    ${UI_FILES}
    ${RESOURCES_FILES}
)
target_link_libraries(${PROJECT_NAME}
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::SerialPort
    Qt${QT_VERSION_MAJOR}::Network
    ${LIBS}
)
if(MSVC)
    target_precompile_headers(${PROJECT_NAME}
        PRIVATE
        ${COMMUNICATION_PATH}/communication_pch.h
    )
endif()
