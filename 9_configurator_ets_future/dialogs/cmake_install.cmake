# Install script for directory: D:/IPS/configurator_ets_dop/dialogs

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

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("D:/IPS/configurator_ets_dop/dialogs/about/cmake_install.cmake")
  include("D:/IPS/configurator_ets_dop/dialogs/stringlist/cmake_install.cmake")
  include("D:/IPS/configurator_ets_dop/dialogs/setupconnectionsdlg/cmake_install.cmake")
  include("D:/IPS/configurator_ets_dop/dialogs/cryptodlg/cmake_install.cmake")
  include("D:/IPS/configurator_ets_dop/dialogs/communication/cmake_install.cmake")
  include("D:/IPS/configurator_ets_dop/dialogs/configdlg/cmake_install.cmake")
  include("D:/IPS/configurator_ets_dop/dialogs/fmwaredlg/cmake_install.cmake")
  include("D:/IPS/configurator_ets_dop/dialogs/msgitemsmodel/cmake_install.cmake")
  include("D:/IPS/configurator_ets_dop/dialogs/debugdlg/cmake_install.cmake")
  include("D:/IPS/configurator_ets_dop/dialogs/servicedlg/cmake_install.cmake")
  include("D:/IPS/configurator_ets_dop/dialogs/network_configuring/cmake_install.cmake")
  include("D:/IPS/configurator_ets_dop/dialogs/network/cmake_install.cmake")
  include("D:/IPS/configurator_ets_dop/dialogs/settings/cmake_install.cmake")
  include("D:/IPS/configurator_ets_dop/dialogs/instruments/test/cmake_install.cmake")
  include("D:/IPS/configurator_ets_dop/dialogs/instruments/firmware/cmake_install.cmake")
  include("D:/IPS/configurator_ets_dop/dialogs/stations/activate_control_lines/cmake_install.cmake")
  include("D:/IPS/configurator_ets_dop/dialogs/message_box/cmake_install.cmake")
  include("D:/IPS/configurator_ets_dop/dialogs/type_list/cmake_install.cmake")
  include("D:/IPS/configurator_ets_dop/dialogs/save_project/cmake_install.cmake")
  include("D:/IPS/configurator_ets_dop/dialogs/send_configuration/cmake_install.cmake")
  include("D:/IPS/configurator_ets_dop/dialogs/limit_rows/cmake_install.cmake")

endif()

