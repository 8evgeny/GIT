project(Configurator)
set(EXECUTABLE_NAME "ips-configurator")
message("-- Configuring "  ${EXECUTABLE_NAME} " app")
include(../cmake/project.cmake)

include_directories(${APP_PATH})
link_directories(${OUTPUT_PATH})

set(ITEMS_PATH ${APP_PATH}/main/classes/base_model/items)
set(INFO_PATH ${ITEMS_PATH}/project_info_item)
set(GROUPS_PATH ${ITEMS_PATH}/groups)
set(STATION_PATH ${ITEMS_PATH}/station_item)
set(STATIONS_PATH ${ITEMS_PATH}/stations_item)
set(GLOBAL_PATH ${ITEMS_PATH}/global_parameters_item)
set(AUDIO_PATH ${APP_PATH}/main/audio)
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

#IF(NOT CMAKE_BUILD_TYPE)
#  SET(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build Type" FORCE)
#ENDIF(NOT CMAKE_BUILD_TYPE)

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
    LinguistTools
    Test
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
    LinguistTools
    Test
    REQUIRED
)

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
    ${APP_PATH}/include/delegates/checkbox_delegate.h

    ${MESSAGES_DIALOGS_PATH}/delete_tracks/delete_tracks_dialog.h
    ${MESSAGES_DIALOGS_PATH}/messages/messages_dialog.h
    ${MESSAGES_DIALOGS_PATH}/messages/messages_dialog_instance.h

    ${MESSAGES_TRACKS}/tracks_model.h
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

    ${STATION_PATH}/station_item.h
    ${STATION_PATH}/model_keys/model_keys.h
    ${STATION_PATH}/model_keys/keys.h
    ${STATION_PATH}/model_keys/key_helper.h
    ${STATION_PATH}/active_lines_serializator/active_lines_serializator.h
    ${STATION_PATH}/station_item_settings/station_item_settings.h
    ${STATION_PATH}/station_item_settings/citgit_helper.h
    ${STATION_PATH}/active_lines_serializator/active_lines_serializator.h
    ${STATION_PATH}/station_item_settings/editor_button.h

    ${SIP_SETTINGS}/sip_settings.h
    ${SIP_SETTINGS}/sip_settings_instance.h

    ${GROUPS_PATH}/groups_item.h
    ${GROUPS_PATH}/groups_instance.h
    ${GROUPS_PATH}/groups_selector/groups_selector.h
    ${GROUPS_PATH}/groups_selector/gid_helper.h
    ${GROUPS_PATH}/models/calls/groups_calls.h
    ${GROUPS_PATH}/models/calls/group_calls/group_calls.h
    ${GROUPS_PATH}/models/digital_stations/view.h
    ${GROUPS_PATH}/models/digital_stations/digital_stations.h

    ${INTERFACES_PATH}/interfaces_item.h
    ${SIP_STATIONS_PATH}/sip_stations_item.h
    ${SIP_STATIONS_PATH}/sip_stations_item_instance.h
    ${SIP_STATIONS_PATH}/model_destination/destination.h
    ${SIP_STATIONS_PATH}/model_destination/model_destination.h
    ${SIP_STATIONS_PATH}/model_rules/model_rules.h

    ${COMMENT}/comment.h
    ${COMMENT}/comment_instance.h

    ${INPUT_CONNECTION_ITEM}/comment_settings/comment_settings_instance.h
    ${INPUT_CONNECTION_ITEM}/comment_settings/comment_settings.h
    ${INPUT_CONNECTION_ITEM}/input_connections_instance.h
    ${OUTPUT_CONNECTION_ITEM}/output_connections_instance.h
    ${INPUT_CONNECTION_ITEM}/input_connections_item.h
    ${OUTPUT_CONNECTION_ITEM}/output_connections_item.h
    ${OUTPUT_CONNECTION_ITEM}/oid_helper.h
    ${OUTPUT_CONNECTION_ITEM}/model_output_connections/output_connection.h
    ${OUTPUT_CONNECTION_ITEM}/model_output_connections/model_output_connections.h
    ${INPUT_CONNECTION_ITEM}/model_input_connections/input_connection.h
    ${INPUT_CONNECTION_ITEM}/model_input_connections/model_input_connections.h

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

    classes/base_types/station_types.h
    classes/base_types/function_types.h
    classes/base_types/duration_types.h
    classes/base_types/sip_types.h
    classes/base_types/protocol_types.h
    classes/base_types/registration_types.h

    login/login.h

    programmer/serializator.h
    programmer/sender_data.h
    programmer/programmer_instance.h
    programmer/host_checker.h
    programmer/transfer/transfer.h
    programmer/log_from_socket.h
    programmer/logs_item.h
    programmer/model_programmer/station_programmer/station_programmer.h
    programmer/model_programmer/model_programmer.h
    programmer/programmer_dialog.h
    programmer/client_server/server/islp_server.h
    programmer/client_server/server/src_server/msgprocessor.hpp
    programmer/client_server/multicaster/mcastRecieve.hpp
    programmer/client_server/multicaster/mcastSend.hpp
    programmer/client_server/multicaster/multicast_socket_in.h
    programmer/client_server/multicaster/multicast_socket_out.h
    programmer/model_programmer/station_programmer/station_programmer.h

    classes/program/program_base_log.h
    classes/program/program_file_log.h
    classes/combobox_algo.h
    classes/base_model/items/root_item.h
    classes/algorithm.h
    classes/binary_defines.h
    classes/double.h
    classes/locale.h
    classes/binary_defines.h
    classes/base_model/base_model.h
    classes/program/program_log.h
    classes/options/language.h
    classes/message_box.h
    classes/json.h
    classes/literals.h
    classes/defines.h
    classes/program.h
    classes/algo.h
    classes/cid.h
    classes/oid.h
    classes/gid.h
    classes/cids_container.h
    base_table.h
    main_window.h
    classes/app.h
	../dialogs/cryptodlg/test_cryptodlg.h
	../dialogs/communication/test_communication.h
	../dialogs/configdlg/test_configuredlg.h
	../dialogs/fmwaredlg/test_fmwaredlg.h
	../dialogs/msgitemsmodel/test_msgtablemodel.h
	../dialogs/debugdlg/test_debugviewdlg.h
	../dialogs/servicedlg/test_serviceviewdlg.h
	classes/base_model/items/stations_item/test_stations_item.h
	classes/base_model/items/station_item/test_station_item.h
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

    ${MESSAGES_DIALOGS_PATH}/delete_tracks/delete_tracks_dialog.cpp
    ${MESSAGES_TRACKS}/tracks_model.cpp
    ${MESSAGES_TRACKS}/tracks_filter_model.cpp
    ${MESSAGES_DIALOGS_PATH}/messages/messages_dialog.cpp

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

    ${INPUT_CONNECTION_ITEM}/input_connections_item.cpp
    ${INPUT_CONNECTION_ITEM}/comment_settings/comment_settings.cpp
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

    programmer/model_programmer/station_programmer/station_programmer.cpp
    programmer/model_programmer/model_programmer.cpp
    programmer/programmer_dialog.cpp
    programmer/log_from_socket.cpp
    programmer/transfer/transfer.cpp
    programmer/host_checker.cpp
    programmer/serializator.cpp
    programmer/sender_data.cpp

    login/login.cpp

    programmer/client_server/server/islp_server.cpp
    programmer/client_server/server/src_server/msgprocessor.cpp
    programmer/client_server/multicaster/mcastRecieve.cpp
    programmer/client_server/multicaster/mcastSend.cpp
    programmer/client_server/multicaster/multicast_socket_in.cpp
    programmer/client_server/multicaster/multicast_socket_out.cpp

    classes/base_model/base_model.cpp
    main_window.cpp
    main.cpp
	../dialogs/cryptodlg/test_cryptodlg.cpp
	../dialogs/communication/test_communication.cpp
	../dialogs/configdlg/test_configuredlg.cpp
	../dialogs/fmwaredlg/test_fmwaredlg.cpp
	../dialogs/msgitemsmodel/test_msgtablemodel.cpp
	../dialogs/debugdlg/test_debugviewdlg.cpp
	../dialogs/servicedlg/test_serviceviewdlg.cpp
	classes/base_model/items/stations_item/test_stations_item.cpp
	classes/base_model/items/station_item/test_station_item.cpp
)
set(FORMS
    ${STATIONS_PATH}/stations_item_settings/stations_item_settings.ui
    ${STATIONS_PATH}/stations_item.ui
    ${STATION_PATH}/station_item_settings/station_item_settings.ui
    ${STATION_PATH}/station_item.ui
    ${INTERFACES_PATH}/interfaces_item.ui
    ${INPUT_CONNECTION_ITEM}/input_connections_item.ui
    ${INPUT_CONNECTION_ITEM}/comment_settings/comment_settings.ui
    ${OUTPUT_CONNECTION_ITEM}/output_connections_item.ui

    ${SIP_SETTINGS}/sip_settings.ui
    ${COMMENT}/comment.ui

    ${GLOBAL_PATH}/global_parameters.ui
    ${GROUPS_PATH}/groups_item.ui
    ${INFO_PATH}/project_info.ui   
    ${SIP_STATIONS_PATH}/sip_stations_item.ui
    ${MESSAGES_DIALOGS_PATH}/delete_tracks/delete_tracks_dialog.ui
    ${MESSAGES_DIALOGS_PATH}/messages/messages_dialog.ui

    login/login.ui

    programmer/programmer_dialog.ui
    main_window.ui
)
set(LIBS
    aboutdlg
    setupconnectionsdlg
    cryptodlg
    configdlg
    fmwaredlg
    msgtablemodel
    debugviewdlg
    serviceviewdlg
    network_configuring
    settingsdlg
    firmwaredlg
    ipstestdlg
    networkdlg
    messageboxdlg
    slistdlg
    typelistdlg
    acldlg
    saveprojectdlg
    limitrowsdlg
    sendconfigurationdlg
)

set_source_files_properties(${TranslationFiles}
    PROPERTIES OUTPUT_LOCATION ${OUTPUT_PATH}/langs)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_wrap_ui(UI_FILES
        ${FORMS}
    )
    qt_add_resources(MOC_RESOURCES
        ${RESOURCES}
    )
    qt_add_translation(TS_FILES ${TranslationFiles})
#    qt_create_translation(TS_FILES ${CMAKE_SOURCE_DIR} ${TranslationFiles})
else()
    qt5_wrap_ui(UI_FILES
        ${FORMS}
    )
    qt5_add_resources(MOC_RESOURCES
        ${RESOURCES}
    )
    qt5_add_translation(TS_FILES ${TranslationFiles})
#    qt5_create_translation(TS_FILES ${CMAKE_SOURCE_DIR} ${TranslationFiles})
endif()

add_executable(${EXECUTABLE_NAME}
    ${GUI_TYPE}
    ${HEADER_FILES}
    ${SOURCE_FILES}
    ${UI_FILES}
    ${MOC_FILES}
    ${MOC_RESOURCES}
    ${FORMS}
    ${RESOURCES_PATH}/a.rc
    ${TS_FILES}
)

target_link_libraries(${EXECUTABLE_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::SerialPort
    Qt${QT_VERSION_MAJOR}::Svg
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::Test
#    CONAN_PKG::libgt-headers
    ${SOCKS_LIBS}
    ${LIBS}
#    "-Wl,--stack,4194304"
)

enable_testing(true)
add_test(NAME ${EXECUTABLE_NAME} COMMAND ${EXECUTABLE_NAME}) 

# Add all libs and app to install
foreach(X IN ITEMS ${LIBS} ${EXECUTABLE_NAME})
    add_to_install(${X})
endforeach()
