project(msgtablemodel)
include(../../cmake/project.cmake)
message("-- msgtablemodel "  ${PROJECT_NAME} " lib")

set(MSGTABLEMODEL_PATH ${APP_PATH}/dialogs/msgitemsmodel)
add_definitions(-DMSGTABLEMODEL_LIBRARY)

include_directories(
    ${APP_PATH}
    ${MSGTABLEMODEL_PATH}
)
find_package(QT NAMES
    Qt5
    Qt6
    COMPONENTS
    Widgets
    REQUIRED
)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
    Widgets
    REQUIRED
)
set(HEADER_FILES
    ${MSGTABLEMODEL_PATH}/msgtablemodel.h
    ${MSGTABLEMODEL_PATH}/msgtablemodel_pch.h
    ${MSGTABLEMODEL_PATH}/msgtablemodel_global.h
)
set(SOURCE_FILES
    ${MSGTABLEMODEL_PATH}/msgtablemodel.cpp    
)

add_library(${PROJECT_NAME} SHARED
    ${HEADER_FILES}
    ${SOURCE_FILES}
)
target_link_libraries(${PROJECT_NAME}
	Qt${QT_VERSION_MAJOR}::Widgets
)
if(MSVC)
    target_precompile_headers(${PROJECT_NAME}
        PRIVATE
        ${MSGTABLEMODEL_PATH}/msgtablemodel_pch.h
    )
endif()
