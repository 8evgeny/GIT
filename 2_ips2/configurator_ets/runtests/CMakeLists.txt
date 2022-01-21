cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(RunTests)

add_definitions(-DVERSION_MAJOR=1)
add_definitions(-DVERSION_MINOR=1)
add_definitions(-DVERSION_PATCH=1)

set(EXECUTABLE_NAME "RunTests")
message("-- Configuring "  ${EXECUTABLE_NAME} " tests")
include(../cmake/project.cmake)

set(COMPANY "GIT")
set(COPYRIGHT "Copyright (c) 2018-2020 GIT. All rights reserved.")
set(IDENTIFIER "git-holding.ru")
set(QT_MINIMUM_VERSION 5.15.2)

find_package(QT NAMES
    Qt5
    Qt6
    COMPONENTS
    Widgets
    Gui
    Network
    SerialPort
    Svg
    Multimedia
    REQUIRED
)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
    Widgets
    Gui
    Network
    SerialPort
    Svg
    Multimedia
    REQUIRED
)

IF(CMAKE_BUILD_TYPE STREQUAL RELEASE)
    MESSAGE("----- RELEASE MODE -----")
ELSE()
    MESSAGE("----- DEBUG MODE -----")
    ADD_DEFINITIONS(-D_TEST)
ENDIF()

include(build/conanbuildinfo.cmake)
conan_basic_setup(NO_OUTPUT_DIRS TARGETS)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_PATH})
set(RUNTESTS_PATH ${CMAKE_CURRENT_LIST_DIR})
set(APP_PATH ${RUNTESTS_PATH}/../)

set(ITEMS_PATH ${APP_PATH}/main/classes/base_model/items)
set(INFO_PATH ${ITEMS_PATH}/project_info_item)
set(GROUPS_PATH ${ITEMS_PATH}/groups)
set(STATION_PATH ${ITEMS_PATH}/station_item)
set(STATIONS_PATH ${ITEMS_PATH}/stations_item)
set(GLOBAL_PATH ${ITEMS_PATH}/global_parameters_item)
set(AUDIO_PATH ${APP_PATH}/main/audio)
set(MAIN_PATH ${APP_PATH}/main)
set(MESSAGES_PATH ${APP_PATH}/main/messages)
set(MESSAGES_DIALOGS_PATH ${MESSAGES_PATH}/dialogs)
set(MESSAGES_CREATOR_PATH ${MESSAGES_PATH}/messages_creator)
set(MESSAGES_TRACKS ${MESSAGES_PATH}/model_tracks)
set(INTERFACES_PATH ${ITEMS_PATH}/interfaces)
set(SIP_STATIONS_PATH ${INTERFACES_PATH}/sip_stations_item)
set(INPUT_CONNECTION_ITEM ${SIP_STATIONS_PATH}/input_connections_item)
set(OUTPUT_CONNECTION_ITEM ${SIP_STATIONS_PATH}/output_connections_item)
set(SIP_SETTINGS ${SIP_STATIONS_PATH}/sip_settings)
set(COMMENT ${OUTPUT_CONNECTION_ITEM}/comment)

set(LIBIPS_PATH ${APP_PATH}/main/programmer/libips)
set(NAMES_PATH ${LIBIPS_PATH}/names)
set(TOOLS_PATH ${LIBIPS_PATH}/tools)
set(ISLP_PATH ${LIBIPS_PATH}/islp)

set(AUDIO_PATH ${APP_PATH}/main/audio)
set(MESSAGES_PATH ${APP_PATH}/main/messages)
set(MESSAGES_DIALOGS_PATH ${MESSAGES_PATH}/dialogs)
set(MESSAGES_CREATOR_PATH ${MESSAGES_PATH}/messages_creator)
set(MESSAGES_TRACKS ${MESSAGES_PATH}/model_tracks)

include_directories(${APP_PATH})
include_directories(${MAIN_PATH})
link_directories(${OUTPUT_PATH})

# Add Dialogs definitions
add_definitions(-DLIMITROWS_DIALOG_LIBRARY)
add_definitions(-DTYPELIST_DIALOG_LIBRARY)
add_definitions(-DACTIVATE_CONTROL_LINES_DIALOG_LIBRARY)
add_definitions(-DSTRINGLIST_DIALOG_LIBRARY)
add_definitions(-DSENDCONFIGURATION_DIALOG_LIBRARY)
add_definitions(-DMESSAGEBOX_LIBRARY)

set(HEADER_FILES
    ${APP_PATH}/include/typedefs.h
    ${APP_PATH}/include/file_types.h
    ${APP_PATH}/include/main_pch.h
    ${APP_PATH}/include/log.h
    ${APP_PATH}/include/view.h
    ${APP_PATH}/include/model.h
    ${APP_PATH}/include/tr.h
    ${APP_PATH}/include/statusbar.h
    ${APP_PATH}/include/error_messages.h
    ${APP_PATH}/include/network/http_request.h
    ${APP_PATH}/include/network/json_worker.h
    ${APP_PATH}/include/settings/settings.h
    ${APP_PATH}/include/settings/settings_group.h
    ${APP_PATH}/include/settings/last_settings.h
    ${APP_PATH}/include/settings/settings_pathes.h
    ${APP_PATH}/include/validators/ip_validator.h
    ${APP_PATH}/include/validators/string_validator.h
    ${APP_PATH}/include/validators/symbol_validator.h
    ${APP_PATH}/include/validators/value_int_validator.h
    ${APP_PATH}/include/validators/activate_control_line_validator.h
    ${APP_PATH}/include/validators/contains_validator.h
    ${APP_PATH}/include/convert/enum.h
    ${APP_PATH}/include/convert/integer.h
    ${APP_PATH}/include/convert/string.h
    ${APP_PATH}/include/widgets/edit_line_button.h
    ${APP_PATH}/include/delegates/combobox_delegate.h
    ${APP_PATH}/include/delegates/view_delegate.h
    ${APP_PATH}/include/delegates/priority_delegate.h
    ${APP_PATH}/include/delegates/ip_delegate.h
    ${APP_PATH}/include/delegates/key_delegate.h
    ${APP_PATH}/include/delegates/cidgt_line_edit_delegate.h
    ${APP_PATH}/include/delegates/name_delegate.h

    ${APP_PATH}/dialogs/stringlist/string_list.h
    ${APP_PATH}/dialogs/stations/activate_control_lines/activate_control_lines.h
    ${APP_PATH}/dialogs/limit_rows/limitrows.h
    ${APP_PATH}/dialogs/send_configuration/send_configuration.h
    ${APP_PATH}/dialogs/message_box/message_box.h

    ${MESSAGES_DIALOGS_PATH}/delete_tracks/delete_tracks_dialog.h
    ${MESSAGES_DIALOGS_PATH}/messages/messages_dialog.h
    ${MESSAGES_DIALOGS_PATH}/messages/messages_dialog_instance.h

    ${MESSAGES_TRACKS}/track.h    

    ${MESSAGES_DIALOGS_PATH}/delete_tracks/delete_tracks_dialog.h
    ${MESSAGES_DIALOGS_PATH}/messages/messages_dialog.h
    ${MESSAGES_DIALOGS_PATH}/messages/messages_dialog_instance.h
    ${MESSAGES_TRACKS}/track.h

    ${INFO_PATH}/model/model_comments.h
    ${INFO_PATH}/model/comment.h
    ${INFO_PATH}/project_info_instance.h
    ${INFO_PATH}/project_info_item.h

    ${GLOBAL_PATH}/global_parameters.h
    ${GLOBAL_PATH}/cid_helper.h
    ${GLOBAL_PATH}/ip_helper.h

    ${STATIONS_PATH}/stations_item.h
    ${STATIONS_PATH}/stations_item_settings/stations_item_settings.h
    ${STATIONS_PATH}/model_stations/station.h
    ${STATIONS_PATH}/model_stations/model_stations.h
    ${STATIONS_PATH}/stations_item_instance.h

    ${SIP_SETTINGS}/sip_settings.h
    ${SIP_SETTINGS}/sip_settings_instance.h

    ${STATION_PATH}/station_item.h
    ${STATION_PATH}/model_keys/model_keys.h
    ${STATION_PATH}/model_keys/keys.h
    ${STATION_PATH}/model_keys/key_helper.h
    ${STATION_PATH}/active_lines_serializator/active_lines_serializator.h
    ${STATION_PATH}/station_item_settings/station_item_settings.h
    ${STATION_PATH}/station_item_settings/citgit_helper.h
    ${STATION_PATH}/active_lines_serializator/active_lines_serializator.h
    ${STATION_PATH}/station_item_settings/editor_button.h
    ${OUTPUT_CONNECTION_ITEM}/oid_helper.h
    ${INPUT_CONNECTION_ITEM}/comment_settings/comment_settings.h
    ${INPUT_CONNECTION_ITEM}/comment_settings/comment_settings_instance.h

    ${GROUPS_PATH}/groups_item.h
    ${GROUPS_PATH}/groups_instance.h
    ${GROUPS_PATH}/groups_selector/groups_selector.h
    ${GROUPS_PATH}/groups_selector/gid_helper.h
    ${GROUPS_PATH}/models/calls/groups_calls.h
    ${GROUPS_PATH}/models/calls/group_calls/group_calls.h
    ${GROUPS_PATH}/models/digital_stations/view.h
    ${GROUPS_PATH}/models/digital_stations/digital_stations.h

    ${LIBIPS_PATH}/debug_macro.h
    ${LIBIPS_PATH}/ips_helpers.hpp
    ${LIBIPS_PATH}/islp.hpp

    ${NAMES_PATH}/ipsnames.hpp
    ${TOOLS_PATH}/arcstylecrypt.hpp
    ${TOOLS_PATH}/intercomstation.hpp
    ${TOOLS_PATH}/nswfl_crc32.hpp

    ${ISLP_PATH}/basicmessage.hpp
    ${ISLP_PATH}/clientsm.hpp
    ${ISLP_PATH}/details.hpp
    ${ISLP_PATH}/doxydoc.hpp
    ${ISLP_PATH}/setstationmessage.hpp
    ${ISLP_PATH}/stationresponse.hpp
    ${ISLP_PATH}/yellmsg.hpp

    ${INTERFACES_PATH}/interfaces_item.h
    ${SIP_STATIONS_PATH}/sip_stations_item.h
    ${SIP_STATIONS_PATH}/sip_stations_item_instance.h
    ${SIP_STATIONS_PATH}/model_destination/destination.h
    ${SIP_STATIONS_PATH}/model_destination/model_destination.h
    ${SIP_STATIONS_PATH}/model_rules/model_rules.h

    ${MAIN_PATH}/classes/base_types/station_types.h
    ${MAIN_PATH}/classes/base_types/function_types.h
    ${MAIN_PATH}/classes/base_types/duration_types.h
    ${MAIN_PATH}/classes/base_types/sip_types.h

    ${MAIN_PATH}/login/login.h

    ${MAIN_PATH}/programmer/serializator.h
    ${MAIN_PATH}/programmer/sender_data.h
    ${MAIN_PATH}/programmer/programmer_instance.h
    ${MAIN_PATH}/programmer/host_checker.h
    ${MAIN_PATH}/programmer/transfer/transfer.h
    ${MAIN_PATH}/programmer/log_from_socket.h
    ${MAIN_PATH}/programmer/logs_item.h
    ${MAIN_PATH}/programmer/model_programmer/station_programmer/station_programmer.h
    ${MAIN_PATH}/programmer/model_programmer/model_programmer.h
    ${MAIN_PATH}/programmer/programmer_dialog.h
    ${MAIN_PATH}/programmer/client_server/server/islp_server.h
    ${MAIN_PATH}/programmer/client_server/server/src_server/msgprocessor.hpp
    ${MAIN_PATH}/programmer/client_server/multicaster/mcastRecieve.hpp
    ${MAIN_PATH}/programmer/client_server/multicaster/mcastSend.hpp
    ${MAIN_PATH}/programmer/client_server/multicaster/multicast_socket_in.h
    ${MAIN_PATH}/programmer/client_server/multicaster/multicast_socket_out.h
    ${MAIN_PATH}/programmer/model_programmer/station_programmer/station_programmer.h

    ${MAIN_PATH}/classes/program/program_base_log.h
    ${MAIN_PATH}/classes/program/program_file_log.h
    ${MAIN_PATH}/classes/combobox_algo.h
    ${MAIN_PATH}/classes/base_model/items/root_item.h
    ${MAIN_PATH}/classes/algorithm.h
    ${MAIN_PATH}/classes/binary_defines.h
    ${MAIN_PATH}/classes/double.h
    ${MAIN_PATH}/classes/locale.h    
    ${MAIN_PATH}/classes/base_model/base_model.h
    ${MAIN_PATH}/classes/program/program_log.h
    ${MAIN_PATH}/classes/options/language.h
    ${MAIN_PATH}/classes/message_box.h
    ${MAIN_PATH}/classes/json.h
    ${MAIN_PATH}/classes/literals.h
    ${MAIN_PATH}/classes/defines.h
    ${MAIN_PATH}/classes/program.h
    ${MAIN_PATH}/classes/algo.h
    ${MAIN_PATH}/classes/cid.h
    ${MAIN_PATH}/classes/oid.h
    ${MAIN_PATH}/classes/gid.h
    ${MAIN_PATH}/classes/cids_container.h
    ${MAIN_PATH}/base_table.h
    ${MAIN_PATH}/classes/app.h
)
set(SOURCE_FILES
    ${APP_PATH}/include/delegates/view_delegate.cpp
    ${APP_PATH}/include/delegates/priority_delegate.cpp
    ${APP_PATH}/include/delegates/ip_delegate.cpp
    ${APP_PATH}/include/delegates/key_delegate.cpp
    ${APP_PATH}/include/delegates/name_delegate.cpp
    ${INFO_PATH}/project_info_item.cpp
    ${INFO_PATH}/model/model_comments.cpp
    ${GLOBAL_PATH}/global_parameters.cpp

    ${APP_PATH}/dialogs/type_list/type_list.cpp
    ${APP_PATH}/dialogs/stringlist/string_list.cpp
    ${APP_PATH}/dialogs/stations/activate_control_lines/activate_control_lines.cpp
    ${APP_PATH}/dialogs/limit_rows/limitrows.cpp
    ${APP_PATH}/dialogs/send_configuration/send_configuration.cpp
    ${APP_PATH}/dialogs/message_box/message_box.cpp

    ${MESSAGES_DIALOGS_PATH}/delete_tracks/delete_tracks_dialog.cpp
    ${MESSAGES_DIALOGS_PATH}/messages/messages_dialog.cpp
    ${MESSAGES_TRACKS}/tracks_model.cpp
    ${MESSAGES_TRACKS}/tracks_filter_model.cpp

    ${STATION_PATH}/station_item.cpp
    ${STATION_PATH}/model_keys/model_keys.cpp
    ${STATION_PATH}/model_keys/key_helper.cpp
    ${STATION_PATH}/station_item_settings/station_item_settings.cpp
    ${STATION_PATH}/active_lines_serializator/active_lines_serializator.cpp

    ${STATIONS_PATH}/stations_item.cpp
    ${STATIONS_PATH}/stations_item_settings/stations_item_settings.cpp
    ${STATIONS_PATH}/model_stations/model_stations.cpp

    ${SIP_SETTINGS}/sip_settings.cpp
    ${COMMENT}/comment.cpp

    ${GROUPS_PATH}/groups_item.cpp
    ${GROUPS_PATH}/groups_selector/groups_selector.cpp
    ${GROUPS_PATH}/groups_selector/gid_helper.cpp
    ${GROUPS_PATH}/models/calls/groups_calls.cpp
    ${GROUPS_PATH}/models/digital_stations/view.cpp
    ${GROUPS_PATH}/models/digital_stations/digital_stations.cpp

    ${INTERFACES_PATH}/interfaces_item.cpp
    ${SIP_STATIONS_PATH}/sip_stations_item.cpp
    ${SIP_STATIONS_PATH}/model_rules/model_rules.cpp
    ${SIP_STATIONS_PATH}/model_destination/model_destination.cpp

    ${INPUT_CONNECTION_ITEM}/comment_settings/comment_settings.cpp
    ${INPUT_CONNECTION_ITEM}/input_connections_item.cpp
    ${OUTPUT_CONNECTION_ITEM}/output_connections_item.cpp
    ${OUTPUT_CONNECTION_ITEM}/oid_helper.cpp
    ${OUTPUT_CONNECTION_ITEM}/model_output_connections/model_output_connections.cpp
    ${INPUT_CONNECTION_ITEM}/model_input_connections/model_input_connections.cpp

    ${GLOBAL_PATH}/cid_helper.cpp
    ${GLOBAL_PATH}/ip_helper.cpp
    ${NAMES_PATH}/ipsnames.cpp
    ${TOOLS_PATH}/arcstylecrypt.cpp
    ${TOOLS_PATH}/intercomstation.cpp
    ${TOOLS_PATH}/nswfl_crc32.cpp

    ${ISLP_PATH}/basicmessage.cpp
    ${ISLP_PATH}/clientsm.cpp
    ${ISLP_PATH}/setstationmessage.cpp
    ${ISLP_PATH}/stationresponse.cpp
    ${ISLP_PATH}/yellmsg.cpp

    ${MAIN_PATH}/programmer/model_programmer/station_programmer/station_programmer.cpp
    ${MAIN_PATH}/programmer/model_programmer/model_programmer.cpp
    ${MAIN_PATH}/programmer/programmer_dialog.cpp
    ${MAIN_PATH}/programmer/log_from_socket.cpp
    ${MAIN_PATH}/programmer/transfer/transfer.cpp
    ${MAIN_PATH}/programmer/host_checker.cpp
    ${MAIN_PATH}/programmer/serializator.cpp
    ${MAIN_PATH}/programmer/sender_data.cpp

    ${MAIN_PATH}/login/login.cpp

    ${MAIN_PATH}/programmer/client_server/server/islp_server.cpp
    ${MAIN_PATH}/programmer/client_server/server/src_server/msgprocessor.cpp
    ${MAIN_PATH}/programmer/client_server/multicaster/mcastRecieve.cpp
    ${MAIN_PATH}/programmer/client_server/multicaster/mcastSend.cpp
    ${MAIN_PATH}/programmer/client_server/multicaster/multicast_socket_in.cpp
    ${MAIN_PATH}/programmer/client_server/multicaster/multicast_socket_out.cpp

    ${MAIN_PATH}/classes/base_model/base_model.cpp
    main.cpp
)
set(FORMS
    ${STATIONS_PATH}/stations_item_settings/stations_item_settings.ui
    ${STATIONS_PATH}/stations_item.ui
    ${STATION_PATH}/station_item_settings/station_item_settings.ui
    ${STATION_PATH}/station_item.ui

    ${APP_PATH}/dialogs/type_list/type_list.ui
    ${APP_PATH}/dialogs/stringlist/string_list.ui
    ${APP_PATH}/dialogs/stations/activate_control_lines/activate_control_lines.ui
    ${APP_PATH}/dialogs/limit_rows/limitrows.ui
    ${APP_PATH}/dialogs/send_configuration/send_configuration.ui
    ${APP_PATH}/dialogs/message_box/message_box.ui

    ${GLOBAL_PATH}/global_parameters.ui
    ${GROUPS_PATH}/groups_item.ui
    ${INFO_PATH}/project_info.ui
    ${SIP_STATIONS_PATH}/sip_stations_item.ui
    ${MESSAGES_DIALOGS_PATH}/delete_tracks/delete_tracks_dialog.ui
    ${MESSAGES_DIALOGS_PATH}/messages/messages_dialog.ui

    ${INTERFACES_PATH}/interfaces_item.ui
    ${INPUT_CONNECTION_ITEM}/input_connections_item.ui
    ${INPUT_CONNECTION_ITEM}/comment_settings/comment_settings.ui
    ${OUTPUT_CONNECTION_ITEM}/output_connections_item.ui
    ${SIP_SETTINGS}/sip_settings.ui
    ${COMMENT}/comment.ui

    ${MAIN_PATH}/login/login.ui
    ${MAIN_PATH}/programmer/programmer_dialog.ui
    ${MESSAGES_DIALOGS_PATH}/delete_tracks/delete_tracks_dialog.ui
    ${MESSAGES_DIALOGS_PATH}/messages/messages_dialog.ui
)
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_wrap_ui(UI_FILES
        ${FORMS}
    )
    qt_add_resources(MOC_RESOURCES
        ${RESOURCES}
    )
else()
    qt5_wrap_ui(UI_FILES
        ${FORMS}
    )
    qt5_add_resources(MOC_RESOURCES
        ${RESOURCES}
    )
endif()

add_executable(${EXECUTABLE_NAME}
    ${HEADER_FILES}
    ${SOURCE_FILES}
    ${UI_FILES}
    ${MOC_FILES}
    ${FORMS}
)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE    
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::SerialPort
    Qt${QT_VERSION_MAJOR}::Svg
    Qt${QT_VERSION_MAJOR}::Multimedia
    CONAN_PKG::libgt-headers
    CONAN_PKG::libruntests-headers
    ${SOCKS_LIBS}
)

add_test(NAME RunTests COMMAND RunTests -r junit)
