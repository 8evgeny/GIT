project(settingsdlg)
include(../../cmake/project.cmake)
message("-- Configuring "  ${PROJECT_NAME} " lib")

set(SETTINGS_PATH ${APP_PATH}/dialogs/settings)
add_definitions(-DSETTINGS_DIALOG_LIBRARY)

include_directories(
    ${APP_PATH}
    ${SETTINGS_PATH}
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
    ${SETTINGS_PATH}/settings.h
    ${SETTINGS_PATH}/settings_pch.h
    ${SETTINGS_PATH}/settings_global.h
)
set(SOURCE_FILES
    ${SETTINGS_PATH}/settings.cpp    
)
set(FORMS
    ${SETTINGS_PATH}/settings.ui
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
        ${SETTINGS_PATH}/settings_pch.h
    )
endif()
