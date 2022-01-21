project(acldlg)
include(../../../cmake/project.cmake)
message("-- Configuring "  ${PROJECT_NAME} " lib")

set(ACL_PATH ${APP_PATH}/dialogs/stations/activate_control_lines)
add_definitions(-DACTIVATE_CONTROL_LINES_DIALOG_LIBRARY)

include_directories(
    ${APP_PATH}
    ${ALC_PATH}
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
    ${APP_PATH}/include/tr.h
    ${APP_PATH}/include/view.h
    ${APP_PATH}/include/log.h
    ${APP_PATH}/include/main_pch.h
    ${APP_PATH}/include/model.h
    ${APP_PATH}/include/widgets/edit_line_button.h
    ${ACL_PATH}/activate_control_lines.h
    ${ACL_PATH}/activate_control_lines_pch.h
    ${ACL_PATH}/activate_control_lines_global.h
)
set(SOURCE_FILES
    ${ACL_PATH}/activate_control_lines.cpp
)
set(FORMS
    ${ACL_PATH}/activate_control_lines.ui
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
    slistdlg
)
if(MSVC)
    target_precompile_headers(${PROJECT_NAME}
        PRIVATE
        ${ACL_PATH}/activate_control_lines_pch.h
    )
endif()
