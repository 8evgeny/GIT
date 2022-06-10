project(network_configuring)
include(../../cmake/project.cmake)
message("-- Configuring "  ${PROJECT_NAME} " lib")

set(NETWORK_CONFIGURING_PATH ${APP_PATH}/dialogs/network_configuring)
add_definitions(-DNETWORK_CONFIGURING_DIALOG_LIBRARY)

include_directories(
    ${APP_PATH}
    ${NETWORK_CONFIGURING_PATH}
)
find_package(QT NAMES
    Qt5
    COMPONENTS
    Widgets
    Gui
    Network
    REQUIRED
)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
    Widgets
    Gui
    Network
    REQUIRED
)
set(HEADER_FILES
    ${NETWORK_CONFIGURING_PATH}/network_configuring.h
    ${NETWORK_CONFIGURING_PATH}/network_configuring_pch.h
    ${NETWORK_CONFIGURING_PATH}/network_configuring_global.h
)
set(SOURCE_FILES
    ${NETWORK_CONFIGURING_PATH}/network_configuring.cpp    
)
set(FORMS
    ${NETWORK_CONFIGURING_PATH}/network_configuring.ui
)

set(LIBS
    slistdlg
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
    Qt${QT_VERSION_MAJOR}::Network
    ${LIBS}
)
if(MSVC)
    target_precompile_headers(${PROJECT_NAME}
        PRIVATE
        ${NETWORK_CONFIGURING_PATH}/network_configuring_pch.h
    )
endif()
