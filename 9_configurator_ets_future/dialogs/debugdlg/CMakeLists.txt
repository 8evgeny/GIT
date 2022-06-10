project(debugviewdlg)
include(../../cmake/project.cmake)
message("-- debugview "  ${PROJECT_NAME} " lib")

set(DEBUGVIEWDLG_PATH ${APP_PATH}/dialogs/debugdlg)
add_definitions(-DDEBUGVIEW_DIALOG_LIBRARY)

include_directories(
    ${APP_PATH}
    ${DEBUGVIEWDLG_PATH}
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
    ${DEBUGVIEWDLG_PATH}/debugviewdlg.h
    ${DEBUGVIEWDLG_PATH}/debugviewdlg_pch.h
    ${DEBUGVIEWDLG_PATH}/debugviewdlg_global.h
)
set(SOURCE_FILES
    ${DEBUGVIEWDLG_PATH}/debugviewdlg.cpp    
)
set(FORMS
    ${DEBUGVIEWDLG_PATH}/debugviewdlg.ui
)

set(LIBS
	cryptodlg
	msgtablemodel
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
    ${LIBS}
)
if(MSVC)
    target_precompile_headers(${PROJECT_NAME}
        PRIVATE
        ${DEBUGVIEWDLG_PATH}/debugviewdlg_pch.h
    )
endif()
