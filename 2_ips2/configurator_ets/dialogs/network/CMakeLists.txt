project(networkdlg)
include(../../cmake/project.cmake)
message("-- Configuring "  ${PROJECT_NAME} " lib")

set(NETWORK_PATH ${APP_PATH}/dialogs/network)
add_definitions(-DNETWORK_DIALOG_LIBRARY)

include_directories(
    ${APP_PATH}
    ${NETWORK_PATH}
)
find_package(QT NAMES
    Qt5
    Qt6
    COMPONENTS
    Widgets
    Gui
    REQUIRED
)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
    Widgets
    Gui
    REQUIRED
)
set(HEADER_FILES
    ${NETWORK_PATH}/network.h
    ${NETWORK_PATH}/network_pch.h
    ${NETWORK_PATH}/network_global.h    
)
set(SOURCE_FILES
    ${NETWORK_PATH}/network.cpp    
)
set(FORMS
    ${NETWORK_PATH}/network.ui
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
)
if(MSVC)
    target_precompile_headers(${PROJECT_NAME}
        PRIVATE
        ${NETWORK_PATH}/network_pch.h
    )
endif()
