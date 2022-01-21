project(fmwaredlg)
include(../../cmake/project.cmake)
message("-- Configuring "  ${PROJECT_NAME} " lib")

set(FMWAREDLG_PATH ${APP_PATH}/dialogs/fmwaredlg)
add_definitions(-DFMWARE_DIALOG_LIBRARY)

include_directories(
    ${APP_PATH}
    ${FMWAREDLG_PATH}
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
    ${FMWAREDLG_PATH}/fmwaredlg.h
    ${FMWAREDLG_PATH}/fmwaredlg_pch.h
    ${FMWAREDLG_PATH}/fmwaredlg_global.h
)
set(SOURCE_FILES
    ${FMWAREDLG_PATH}/fmwaredlg.cpp    
)
set(FORMS
    ${FMWAREDLG_PATH}/fmwaredlg.ui
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
        ${FMWAREDLG_PATH}/fmwaredlg_pch.h
    )
endif()
