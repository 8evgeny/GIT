project(typelistdlg)
include(../../cmake/project.cmake)
message("-- Configuring "  ${PROJECT_NAME} " lib")

set(TYPELIST_PATH ${APP_PATH}/dialogs/type_list)
add_definitions(-DTYPELIST_DIALOG_LIBRARY)

include_directories(
    ${APP_PATH}
    ${TYPELIST_PATH}
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
    ${APP_PATH}/include/view.h
    ${APP_PATH}/include/log.h
    ${APP_PATH}/include/model.h
    ${APP_PATH}/include/main_pch.h
    ${TYPELIST_PATH}/type_list.h
    ${TYPELIST_PATH}/type_list_pch.h
    ${TYPELIST_PATH}/type_list_global.h
)
set(SOURCE_FILES
    ${TYPELIST_PATH}/type_list.cpp    
)
set(FORMS
    ${TYPELIST_PATH}/type_list.ui
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
        ${TYPELIST_PATH}/type_list_pch.h
    )
endif()
