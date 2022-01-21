project(cryptodlg)
include(../../cmake/project.cmake)
message("-- Configuring "  ${PROJECT_NAME} " lib")

set(CRYPTO_PATH ${APP_PATH}/dialogs/cryptodlg)
add_definitions(-DCRYPTO_DIALOG_LIBRARY)

include_directories(
    ${APP_PATH}
    ${CRYPTO_PATH}
)
find_package(QT NAMES
    Qt5
    COMPONENTS
    Widgets
    Gui
	Test
    REQUIRED
)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
    Widgets
    Gui
	Test
    REQUIRED
)
set(HEADER_FILES
    ${CRYPTO_PATH}/cryptodlg.h
    ${CRYPTO_PATH}/cryptodlg_pch.h
    ${CRYPTO_PATH}/cryptodlg_global.h
)
set(SOURCE_FILES
    ${CRYPTO_PATH}/cryptodlg.cpp    
)


add_library(${PROJECT_NAME} SHARED
    ${HEADER_FILES}
    ${SOURCE_FILES}
    ${UI_FILES}
    ${RESOURCES_FILES}
)
target_link_libraries(${PROJECT_NAME}
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Gui
	Qt${QT_VERSION_MAJOR}::Test
    ${LIBS}
)
if(MSVC)
    target_precompile_headers(${PROJECT_NAME}
        PRIVATE
        ${CRYPTO_PATH}/cryptodlg_pch.h
    )
endif()
