project(configdlg)
include(../../cmake/project.cmake)
message("-- Configuring "  ${PROJECT_NAME} " lib")

set(CONFIGUREDLG_PATH ${APP_PATH}/dialogs/configdlg)
add_definitions(-DCONFIGURE_DIALOG_LIBRARY)

include_directories(
    ${APP_PATH}
    ${CONFIGUREDLG_PATH}
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
    ${CONFIGUREDLG_PATH}/configuredlg.h
    ${CONFIGUREDLG_PATH}/configuredlg_pch.h
    ${CONFIGUREDLG_PATH}/configuredlg_global.h
)
set(SOURCE_FILES
    ${CONFIGUREDLG_PATH}/configuredlg.cpp    
)
set(FORMS
    ${CONFIGUREDLG_PATH}/configuredlg.ui
)

set(LIBS
    slistdlg
	cryptodlg
    communication
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
    ${LIBS}
)
if(MSVC)
    target_precompile_headers(${PROJECT_NAME}
        PRIVATE
        ${CONFIGUREDLG_PATH}/configuredlg_pch.h
    )
endif()
