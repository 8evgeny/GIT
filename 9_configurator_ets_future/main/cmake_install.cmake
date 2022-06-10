# Install script for directory: D:/IPS/configurator_ets_dop/main

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/IPSConfigurator")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/QT5.15MinGW810/mingw64/bin/objdump.exe")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xaboutdlgx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/IPS/configurator_ets_dop/dialogs/about/libaboutdlg.dll.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xaboutdlgx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE SHARED_LIBRARY FILES "D:/IPS/configurator_ets_dop/bin_win10/libaboutdlg.dll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libaboutdlg.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libaboutdlg.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/QT5.15MinGW810/mingw64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libaboutdlg.dll")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xsetupconnectionsdlgx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/IPS/configurator_ets_dop/dialogs/setupconnectionsdlg/libsetupconnectionsdlg.dll.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xsetupconnectionsdlgx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE SHARED_LIBRARY FILES "D:/IPS/configurator_ets_dop/bin_win10/libsetupconnectionsdlg.dll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libsetupconnectionsdlg.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libsetupconnectionsdlg.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/QT5.15MinGW810/mingw64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libsetupconnectionsdlg.dll")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xcryptodlgx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/IPS/configurator_ets_dop/dialogs/cryptodlg/libcryptodlg.dll.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xcryptodlgx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE SHARED_LIBRARY FILES "D:/IPS/configurator_ets_dop/bin_win10/libcryptodlg.dll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libcryptodlg.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libcryptodlg.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/QT5.15MinGW810/mingw64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libcryptodlg.dll")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xconfigdlgx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/IPS/configurator_ets_dop/dialogs/configdlg/libconfigdlg.dll.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xconfigdlgx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE SHARED_LIBRARY FILES "D:/IPS/configurator_ets_dop/bin_win10/libconfigdlg.dll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libconfigdlg.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libconfigdlg.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/QT5.15MinGW810/mingw64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libconfigdlg.dll")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xfmwaredlgx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/IPS/configurator_ets_dop/dialogs/fmwaredlg/libfmwaredlg.dll.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xfmwaredlgx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE SHARED_LIBRARY FILES "D:/IPS/configurator_ets_dop/bin_win10/libfmwaredlg.dll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libfmwaredlg.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libfmwaredlg.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/QT5.15MinGW810/mingw64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libfmwaredlg.dll")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xmsgtablemodelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/IPS/configurator_ets_dop/dialogs/msgitemsmodel/libmsgtablemodel.dll.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xmsgtablemodelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE SHARED_LIBRARY FILES "D:/IPS/configurator_ets_dop/bin_win10/libmsgtablemodel.dll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libmsgtablemodel.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libmsgtablemodel.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/QT5.15MinGW810/mingw64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libmsgtablemodel.dll")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdebugviewdlgx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/IPS/configurator_ets_dop/dialogs/debugdlg/libdebugviewdlg.dll.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdebugviewdlgx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE SHARED_LIBRARY FILES "D:/IPS/configurator_ets_dop/bin_win10/libdebugviewdlg.dll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libdebugviewdlg.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libdebugviewdlg.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/QT5.15MinGW810/mingw64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libdebugviewdlg.dll")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xserviceviewdlgx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/IPS/configurator_ets_dop/dialogs/servicedlg/libserviceviewdlg.dll.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xserviceviewdlgx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE SHARED_LIBRARY FILES "D:/IPS/configurator_ets_dop/bin_win10/libserviceviewdlg.dll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libserviceviewdlg.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libserviceviewdlg.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/QT5.15MinGW810/mingw64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libserviceviewdlg.dll")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xnetwork_configuringx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/IPS/configurator_ets_dop/dialogs/network_configuring/libnetwork_configuring.dll.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xnetwork_configuringx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE SHARED_LIBRARY FILES "D:/IPS/configurator_ets_dop/bin_win10/libnetwork_configuring.dll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libnetwork_configuring.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libnetwork_configuring.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/QT5.15MinGW810/mingw64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libnetwork_configuring.dll")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xsettingsdlgx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/IPS/configurator_ets_dop/dialogs/settings/libsettingsdlg.dll.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xsettingsdlgx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE SHARED_LIBRARY FILES "D:/IPS/configurator_ets_dop/bin_win10/libsettingsdlg.dll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libsettingsdlg.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libsettingsdlg.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/QT5.15MinGW810/mingw64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libsettingsdlg.dll")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xfirmwaredlgx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/IPS/configurator_ets_dop/dialogs/instruments/firmware/libfirmwaredlg.dll.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xfirmwaredlgx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE SHARED_LIBRARY FILES "D:/IPS/configurator_ets_dop/bin_win10/libfirmwaredlg.dll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libfirmwaredlg.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libfirmwaredlg.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/QT5.15MinGW810/mingw64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libfirmwaredlg.dll")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xipstestdlgx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/IPS/configurator_ets_dop/dialogs/instruments/test/libipstestdlg.dll.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xipstestdlgx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE SHARED_LIBRARY FILES "D:/IPS/configurator_ets_dop/bin_win10/libipstestdlg.dll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libipstestdlg.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libipstestdlg.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/QT5.15MinGW810/mingw64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libipstestdlg.dll")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xnetworkdlgx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/IPS/configurator_ets_dop/dialogs/network/libnetworkdlg.dll.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xnetworkdlgx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE SHARED_LIBRARY FILES "D:/IPS/configurator_ets_dop/bin_win10/libnetworkdlg.dll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libnetworkdlg.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libnetworkdlg.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/QT5.15MinGW810/mingw64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libnetworkdlg.dll")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xmessageboxdlgx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/IPS/configurator_ets_dop/dialogs/message_box/libmessageboxdlg.dll.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xmessageboxdlgx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE SHARED_LIBRARY FILES "D:/IPS/configurator_ets_dop/bin_win10/libmessageboxdlg.dll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libmessageboxdlg.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libmessageboxdlg.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/QT5.15MinGW810/mingw64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libmessageboxdlg.dll")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xslistdlgx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/IPS/configurator_ets_dop/dialogs/stringlist/libslistdlg.dll.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xslistdlgx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE SHARED_LIBRARY FILES "D:/IPS/configurator_ets_dop/bin_win10/libslistdlg.dll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libslistdlg.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libslistdlg.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/QT5.15MinGW810/mingw64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libslistdlg.dll")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xtypelistdlgx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/IPS/configurator_ets_dop/dialogs/type_list/libtypelistdlg.dll.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xtypelistdlgx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE SHARED_LIBRARY FILES "D:/IPS/configurator_ets_dop/bin_win10/libtypelistdlg.dll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libtypelistdlg.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libtypelistdlg.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/QT5.15MinGW810/mingw64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libtypelistdlg.dll")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xacldlgx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/IPS/configurator_ets_dop/dialogs/stations/activate_control_lines/libacldlg.dll.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xacldlgx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE SHARED_LIBRARY FILES "D:/IPS/configurator_ets_dop/bin_win10/libacldlg.dll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libacldlg.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libacldlg.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/QT5.15MinGW810/mingw64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libacldlg.dll")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xsaveprojectdlgx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/IPS/configurator_ets_dop/dialogs/save_project/libsaveprojectdlg.dll.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xsaveprojectdlgx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE SHARED_LIBRARY FILES "D:/IPS/configurator_ets_dop/bin_win10/libsaveprojectdlg.dll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libsaveprojectdlg.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libsaveprojectdlg.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/QT5.15MinGW810/mingw64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libsaveprojectdlg.dll")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlimitrowsdlgx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/IPS/configurator_ets_dop/dialogs/limit_rows/liblimitrowsdlg.dll.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlimitrowsdlgx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE SHARED_LIBRARY FILES "D:/IPS/configurator_ets_dop/bin_win10/liblimitrowsdlg.dll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/liblimitrowsdlg.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/liblimitrowsdlg.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/QT5.15MinGW810/mingw64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/liblimitrowsdlg.dll")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xsendconfigurationdlgx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/IPS/configurator_ets_dop/dialogs/send_configuration/libsendconfigurationdlg.dll.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xsendconfigurationdlgx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE SHARED_LIBRARY FILES "D:/IPS/configurator_ets_dop/bin_win10/libsendconfigurationdlg.dll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libsendconfigurationdlg.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libsendconfigurationdlg.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/QT5.15MinGW810/mingw64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/libsendconfigurationdlg.dll")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xips-configuratorx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator" TYPE EXECUTABLE FILES "D:/IPS/configurator_ets_dop/bin_win10/ips-configurator.exe")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/ips-configurator.exe" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/ips-configurator.exe")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/QT5.15MinGW810/mingw64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/git/ips/configurator/ips-configurator.exe")
    endif()
  endif()
endif()

