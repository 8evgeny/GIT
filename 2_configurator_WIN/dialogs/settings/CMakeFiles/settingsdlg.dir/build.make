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
include dialogs/settings/CMakeFiles/settingsdlg.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include dialogs/settings/CMakeFiles/settingsdlg.dir/compiler_depend.make

# Include the progress variables for this target.
include dialogs/settings/CMakeFiles/settingsdlg.dir/progress.make

# Include the compile flags for this target's objects.
include dialogs/settings/CMakeFiles/settingsdlg.dir/flags.make

dialogs/settings/ui_settings.h: dialogs/settings/settings.ui
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=D:\IPS\configurator_ets_dop\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating ui_settings.h"
	cd /d D:\IPS\configurator_ets_dop\dialogs\settings && C:\QT5.15MinGW810\Qt515\bin\uic.exe -o D:/IPS/configurator_ets_dop/dialogs/settings/ui_settings.h D:/IPS/configurator_ets_dop/dialogs/settings/settings.ui

dialogs/settings/CMakeFiles/settingsdlg.dir/settingsdlg_autogen/mocs_compilation.cpp.obj: dialogs/settings/CMakeFiles/settingsdlg.dir/flags.make
dialogs/settings/CMakeFiles/settingsdlg.dir/settingsdlg_autogen/mocs_compilation.cpp.obj: dialogs/settings/CMakeFiles/settingsdlg.dir/includes_CXX.rsp
dialogs/settings/CMakeFiles/settingsdlg.dir/settingsdlg_autogen/mocs_compilation.cpp.obj: dialogs/settings/settingsdlg_autogen/mocs_compilation.cpp
dialogs/settings/CMakeFiles/settingsdlg.dir/settingsdlg_autogen/mocs_compilation.cpp.obj: dialogs/settings/CMakeFiles/settingsdlg.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\IPS\configurator_ets_dop\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object dialogs/settings/CMakeFiles/settingsdlg.dir/settingsdlg_autogen/mocs_compilation.cpp.obj"
	cd /d D:\IPS\configurator_ets_dop\dialogs\settings && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT dialogs/settings/CMakeFiles/settingsdlg.dir/settingsdlg_autogen/mocs_compilation.cpp.obj -MF CMakeFiles\settingsdlg.dir\settingsdlg_autogen\mocs_compilation.cpp.obj.d -o CMakeFiles\settingsdlg.dir\settingsdlg_autogen\mocs_compilation.cpp.obj -c D:\IPS\configurator_ets_dop\dialogs\settings\settingsdlg_autogen\mocs_compilation.cpp

dialogs/settings/CMakeFiles/settingsdlg.dir/settingsdlg_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/settingsdlg.dir/settingsdlg_autogen/mocs_compilation.cpp.i"
	cd /d D:\IPS\configurator_ets_dop\dialogs\settings && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\IPS\configurator_ets_dop\dialogs\settings\settingsdlg_autogen\mocs_compilation.cpp > CMakeFiles\settingsdlg.dir\settingsdlg_autogen\mocs_compilation.cpp.i

dialogs/settings/CMakeFiles/settingsdlg.dir/settingsdlg_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/settingsdlg.dir/settingsdlg_autogen/mocs_compilation.cpp.s"
	cd /d D:\IPS\configurator_ets_dop\dialogs\settings && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\IPS\configurator_ets_dop\dialogs\settings\settingsdlg_autogen\mocs_compilation.cpp -o CMakeFiles\settingsdlg.dir\settingsdlg_autogen\mocs_compilation.cpp.s

dialogs/settings/CMakeFiles/settingsdlg.dir/settings.cpp.obj: dialogs/settings/CMakeFiles/settingsdlg.dir/flags.make
dialogs/settings/CMakeFiles/settingsdlg.dir/settings.cpp.obj: dialogs/settings/CMakeFiles/settingsdlg.dir/includes_CXX.rsp
dialogs/settings/CMakeFiles/settingsdlg.dir/settings.cpp.obj: dialogs/settings/settings.cpp
dialogs/settings/CMakeFiles/settingsdlg.dir/settings.cpp.obj: dialogs/settings/CMakeFiles/settingsdlg.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\IPS\configurator_ets_dop\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object dialogs/settings/CMakeFiles/settingsdlg.dir/settings.cpp.obj"
	cd /d D:\IPS\configurator_ets_dop\dialogs\settings && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT dialogs/settings/CMakeFiles/settingsdlg.dir/settings.cpp.obj -MF CMakeFiles\settingsdlg.dir\settings.cpp.obj.d -o CMakeFiles\settingsdlg.dir\settings.cpp.obj -c D:\IPS\configurator_ets_dop\dialogs\settings\settings.cpp

dialogs/settings/CMakeFiles/settingsdlg.dir/settings.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/settingsdlg.dir/settings.cpp.i"
	cd /d D:\IPS\configurator_ets_dop\dialogs\settings && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\IPS\configurator_ets_dop\dialogs\settings\settings.cpp > CMakeFiles\settingsdlg.dir\settings.cpp.i

dialogs/settings/CMakeFiles/settingsdlg.dir/settings.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/settingsdlg.dir/settings.cpp.s"
	cd /d D:\IPS\configurator_ets_dop\dialogs\settings && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\IPS\configurator_ets_dop\dialogs\settings\settings.cpp -o CMakeFiles\settingsdlg.dir\settings.cpp.s

# Object files for target settingsdlg
settingsdlg_OBJECTS = \
"CMakeFiles/settingsdlg.dir/settingsdlg_autogen/mocs_compilation.cpp.obj" \
"CMakeFiles/settingsdlg.dir/settings.cpp.obj"

# External object files for target settingsdlg
settingsdlg_EXTERNAL_OBJECTS =

bin_win10/libsettingsdlg.dll: dialogs/settings/CMakeFiles/settingsdlg.dir/settingsdlg_autogen/mocs_compilation.cpp.obj
bin_win10/libsettingsdlg.dll: dialogs/settings/CMakeFiles/settingsdlg.dir/settings.cpp.obj
bin_win10/libsettingsdlg.dll: dialogs/settings/CMakeFiles/settingsdlg.dir/build.make
bin_win10/libsettingsdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Widgets.a
bin_win10/libsettingsdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Gui.a
bin_win10/libsettingsdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Core.a
bin_win10/libsettingsdlg.dll: dialogs/settings/CMakeFiles/settingsdlg.dir/linklibs.rsp
bin_win10/libsettingsdlg.dll: dialogs/settings/CMakeFiles/settingsdlg.dir/objects1.rsp
bin_win10/libsettingsdlg.dll: dialogs/settings/CMakeFiles/settingsdlg.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\IPS\configurator_ets_dop\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX shared library ..\..\bin_win10\libsettingsdlg.dll"
	cd /d D:\IPS\configurator_ets_dop\dialogs\settings && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\settingsdlg.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
dialogs/settings/CMakeFiles/settingsdlg.dir/build: bin_win10/libsettingsdlg.dll
.PHONY : dialogs/settings/CMakeFiles/settingsdlg.dir/build

dialogs/settings/CMakeFiles/settingsdlg.dir/clean:
	cd /d D:\IPS\configurator_ets_dop\dialogs\settings && $(CMAKE_COMMAND) -P CMakeFiles\settingsdlg.dir\cmake_clean.cmake
.PHONY : dialogs/settings/CMakeFiles/settingsdlg.dir/clean

dialogs/settings/CMakeFiles/settingsdlg.dir/depend: dialogs/settings/ui_settings.h
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\IPS\configurator_ets_dop D:\IPS\configurator_ets_dop\dialogs\settings D:\IPS\configurator_ets_dop D:\IPS\configurator_ets_dop\dialogs\settings D:\IPS\configurator_ets_dop\dialogs\settings\CMakeFiles\settingsdlg.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : dialogs/settings/CMakeFiles/settingsdlg.dir/depend

