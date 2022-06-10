project(messageboxdlg)
include(../../cmake/project.cmake)
message("-- Configuring "  ${PROJECT_NAME} " lib")

set(MESSAGEBOX_PATH ${APP_PATH}/dialogs/message_box)
add_definitions(-DMESSAGEBOX_LIBRARY)

include_directories(
    ${APP_PATH}
    ${MESSAGEBOX_PATH}
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
    ${MESSAGEBOX_PATH}/message_box.h
    ${MESSAGEBOX_PATH}/message_box_pch.h
    ${MESSAGEBOX_PATH}/message_box_global.h    
)
set(SOURCE_FILES
    ${MESSAGEBOX_PATH}/message_box.cpp    
)
set(FORMS
    ${MESSAGEBOX_PATH}/message_box.ui
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
        ${MESSAGEBOX_PATH}/message_box_pch.h
    )
endif()
