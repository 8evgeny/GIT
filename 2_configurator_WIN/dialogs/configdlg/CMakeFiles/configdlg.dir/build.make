# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.21

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\IPS\configurator_ets_dop

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\IPS\configurator_ets_dop

# Include any dependencies generated for this target.
include dialogs/configdlg/CMakeFiles/configdlg.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include dialogs/configdlg/CMakeFiles/configdlg.dir/compiler_depend.make

# Include the progress variables for this target.
include dialogs/configdlg/CMakeFiles/configdlg.dir/progress.make

# Include the compile flags for this target's objects.
include dialogs/configdlg/CMakeFiles/configdlg.dir/flags.make

dialogs/configdlg/ui_configuredlg.h: dialogs/configdlg/configuredlg.ui
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=D:\IPS\configurator_ets_dop\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating ui_configuredlg.h"
	cd /d D:\IPS\configurator_ets_dop\dialogs\configdlg && C:\QT5.15MinGW810\Qt515\bin\uic.exe -o D:/IPS/configurator_ets_dop/dialogs/configdlg/ui_configuredlg.h D:/IPS/configurator_ets_dop/dialogs/configdlg/configuredlg.ui

dialogs/configdlg/CMakeFiles/configdlg.dir/configdlg_autogen/mocs_compilation.cpp.obj: dialogs/configdlg/CMakeFiles/configdlg.dir/flags.make
dialogs/configdlg/CMakeFiles/configdlg.dir/configdlg_autogen/mocs_compilation.cpp.obj: dialogs/configdlg/CMakeFiles/configdlg.dir/includes_CXX.rsp
dialogs/configdlg/CMakeFiles/configdlg.dir/configdlg_autogen/mocs_compilation.cpp.obj: dialogs/configdlg/configdlg_autogen/mocs_compilation.cpp
dialogs/configdlg/CMakeFiles/configdlg.dir/configdlg_autogen/mocs_compilation.cpp.obj: dialogs/configdlg/CMakeFiles/configdlg.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\IPS\configurator_ets_dop\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object dialogs/configdlg/CMakeFiles/configdlg.dir/configdlg_autogen/mocs_compilation.cpp.obj"
	cd /d D:\IPS\configurator_ets_dop\dialogs\configdlg && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT dialogs/configdlg/CMakeFiles/configdlg.dir/configdlg_autogen/mocs_compilation.cpp.obj -MF CMakeFiles\configdlg.dir\configdlg_autogen\mocs_compilation.cpp.obj.d -o CMakeFiles\configdlg.dir\configdlg_autogen\mocs_compilation.cpp.obj -c D:\IPS\configurator_ets_dop\dialogs\configdlg\configdlg_autogen\mocs_compilation.cpp

dialogs/configdlg/CMakeFiles/configdlg.dir/configdlg_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/configdlg.dir/configdlg_autogen/mocs_compilation.cpp.i"
	cd /d D:\IPS\configurator_ets_dop\dialogs\configdlg && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\IPS\configurator_ets_dop\dialogs\configdlg\configdlg_autogen\mocs_compilation.cpp > CMakeFiles\configdlg.dir\configdlg_autogen\mocs_compilation.cpp.i

dialogs/configdlg/CMakeFiles/configdlg.dir/configdlg_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/configdlg.dir/configdlg_autogen/mocs_compilation.cpp.s"
	cd /d D:\IPS\configurator_ets_dop\dialogs\configdlg && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\IPS\configurator_ets_dop\dialogs\configdlg\configdlg_autogen\mocs_compilation.cpp -o CMakeFiles\configdlg.dir\configdlg_autogen\mocs_compilation.cpp.s

dialogs/configdlg/CMakeFiles/configdlg.dir/configuredlg.cpp.obj: dialogs/configdlg/CMakeFiles/configdlg.dir/flags.make
dialogs/configdlg/CMakeFiles/configdlg.dir/configuredlg.cpp.obj: dialogs/configdlg/CMakeFiles/configdlg.dir/includes_CXX.rsp
dialogs/configdlg/CMakeFiles/configdlg.dir/configuredlg.cpp.obj: dialogs/configdlg/configuredlg.cpp
dialogs/configdlg/CMakeFiles/configdlg.dir/configuredlg.cpp.obj: dialogs/configdlg/CMakeFiles/configdlg.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\IPS\configurator_ets_dop\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object dialogs/configdlg/CMakeFiles/configdlg.dir/configuredlg.cpp.obj"
	cd /d D:\IPS\configurator_ets_dop\dialogs\configdlg && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT dialogs/configdlg/CMakeFiles/configdlg.dir/configuredlg.cpp.obj -MF CMakeFiles\configdlg.dir\configuredlg.cpp.obj.d -o CMakeFiles\configdlg.dir\configuredlg.cpp.obj -c D:\IPS\configurator_ets_dop\dialogs\configdlg\configuredlg.cpp

dialogs/configdlg/CMakeFiles/configdlg.dir/configuredlg.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/configdlg.dir/configuredlg.cpp.i"
	cd /d D:\IPS\configurator_ets_dop\dialogs\configdlg && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\IPS\configurator_ets_dop\dialogs\configdlg\configuredlg.cpp > CMakeFiles\configdlg.dir\configuredlg.cpp.i

dialogs/configdlg/CMakeFiles/configdlg.dir/configuredlg.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/configdlg.dir/configuredlg.cpp.s"
	cd /d D:\IPS\configurator_ets_dop\dialogs\configdlg && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\IPS\configurator_ets_dop\dialogs\configdlg\configuredlg.cpp -o CMakeFiles\configdlg.dir\configuredlg.cpp.s

# Object files for target configdlg
configdlg_OBJECTS = \
"CMakeFiles/configdlg.dir/configdlg_autogen/mocs_compilation.cpp.obj" \
"CMakeFiles/configdlg.dir/configuredlg.cpp.obj"

# External object files for target configdlg
configdlg_EXTERNAL_OBJECTS =

bin_win10/libconfigdlg.dll: dialogs/configdlg/CMakeFiles/configdlg.dir/configdlg_autogen/mocs_compilation.cpp.obj
bin_win10/libconfigdlg.dll: dialogs/configdlg/CMakeFiles/configdlg.dir/configuredlg.cpp.obj
bin_win10/libconfigdlg.dll: dialogs/configdlg/CMakeFiles/configdlg.dir/build.make
bin_win10/libconfigdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Widgets.a
bin_win10/libconfigdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Gui.a
bin_win10/libconfigdlg.dll: dialogs/stringlist/libslistdlg.dll.a
bin_win10/libconfigdlg.dll: dialogs/communication/libcommunication.dll.a
bin_win10/libconfigdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Core.a
bin_win10/libconfigdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Widgets.a
bin_win10/libconfigdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Gui.a
bin_win10/libconfigdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Core.a
bin_win10/libconfigdlg.dll: dialogs/cryptodlg/libcryptodlg.dll.a
bin_win10/libconfigdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Widgets.a
bin_win10/libconfigdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Gui.a
bin_win10/libconfigdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Test.a
bin_win10/libconfigdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Core.a
bin_win10/libconfigdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Widgets.a
bin_win10/libconfigdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Gui.a
bin_win10/libconfigdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5SerialPort.a
bin_win10/libconfigdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Network.a
bin_win10/libconfigdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Core.a
bin_win10/libconfigdlg.dll: dialogs/configdlg/CMakeFiles/configdlg.dir/linklibs.rsp
bin_win10/libconfigdlg.dll: dialogs/configdlg/CMakeFiles/configdlg.dir/objects1.rsp
bin_win10/libconfigdlg.dll: dialogs/configdlg/CMakeFiles/configdlg.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\IPS\configurator_ets_dop\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX shared library ..\..\bin_win10\libconfigdlg.dll"
	cd /d D:\IPS\configurator_ets_dop\dialogs\configdlg && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\configdlg.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
dialogs/configdlg/CMakeFiles/configdlg.dir/build: bin_win10/libconfigdlg.dll
.PHONY : dialogs/configdlg/CMakeFiles/configdlg.dir/build

dialogs/configdlg/CMakeFiles/configdlg.dir/clean:
	cd /d D:\IPS\configurator_ets_dop\dialogs\configdlg && $(CMAKE_COMMAND) -P CMakeFiles\configdlg.dir\cmake_clean.cmake
.PHONY : dialogs/configdlg/CMakeFiles/configdlg.dir/clean

dialogs/configdlg/CMakeFiles/configdlg.dir/depend: dialogs/configdlg/ui_configuredlg.h
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\IPS\configurator_ets_dop D:\IPS\configurator_ets_dop\dialogs\configdlg D:\IPS\configurator_ets_dop D:\IPS\configurator_ets_dop\dialogs\configdlg D:\IPS\configurator_ets_dop\dialogs\configdlg\CMakeFiles\configdlg.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : dialogs/configdlg/CMakeFiles/configdlg.dir/depend

