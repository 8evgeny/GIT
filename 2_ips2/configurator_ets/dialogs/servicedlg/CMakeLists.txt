project(serviceviewdlg)
include(../../cmake/project.cmake)
message("-- serviceview "  ${PROJECT_NAME} " lib")

set(SERVICEVIEWDLG_PATH ${APP_PATH}/dialogs/servicedlg)
add_definitions(-DSERVICEVIEW_DIALOG_LIBRARY)

include_directories(
    ${APP_PATH}
    ${SERVICEVIEWDLG_PATH}
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
    ${SERVICEVIEWDLG_PATH}/serviceviewdlg.h
    ${SERVICEVIEWDLG_PATH}/serviceviewdlg_pch.h
    ${SERVICEVIEWDLG_PATH}/serviceviewdlg_global.h
)
set(SOURCE_FILES
    ${SERVICEVIEWDLG_PATH}/serviceviewdlg.cpp    
)
set(FORMS
    ${SERVICEVIEWDLG_PATH}/serviceviewdlg.ui
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
        ${SERVICEVIEWDLG_PATH}/serviceviewdlg_pch.h
    )
endif()
