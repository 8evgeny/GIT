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
include dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/compiler_depend.make

# Include the progress variables for this target.
include dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/progress.make

# Include the compile flags for this target's objects.
include dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/flags.make

dialogs/servicedlg/ui_serviceviewdlg.h: dialogs/servicedlg/serviceviewdlg.ui
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=D:\IPS\configurator_ets_dop\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating ui_serviceviewdlg.h"
	cd /d D:\IPS\configurator_ets_dop\dialogs\servicedlg && C:\QT5.15MinGW810\Qt515\bin\uic.exe -o D:/IPS/configurator_ets_dop/dialogs/servicedlg/ui_serviceviewdlg.h D:/IPS/configurator_ets_dop/dialogs/servicedlg/serviceviewdlg.ui

dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/serviceviewdlg_autogen/mocs_compilation.cpp.obj: dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/flags.make
dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/serviceviewdlg_autogen/mocs_compilation.cpp.obj: dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/includes_CXX.rsp
dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/serviceviewdlg_autogen/mocs_compilation.cpp.obj: dialogs/servicedlg/serviceviewdlg_autogen/mocs_compilation.cpp
dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/serviceviewdlg_autogen/mocs_compilation.cpp.obj: dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\IPS\configurator_ets_dop\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/serviceviewdlg_autogen/mocs_compilation.cpp.obj"
	cd /d D:\IPS\configurator_ets_dop\dialogs\servicedlg && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/serviceviewdlg_autogen/mocs_compilation.cpp.obj -MF CMakeFiles\serviceviewdlg.dir\serviceviewdlg_autogen\mocs_compilation.cpp.obj.d -o CMakeFiles\serviceviewdlg.dir\serviceviewdlg_autogen\mocs_compilation.cpp.obj -c D:\IPS\configurator_ets_dop\dialogs\servicedlg\serviceviewdlg_autogen\mocs_compilation.cpp

dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/serviceviewdlg_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serviceviewdlg.dir/serviceviewdlg_autogen/mocs_compilation.cpp.i"
	cd /d D:\IPS\configurator_ets_dop\dialogs\servicedlg && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\IPS\configurator_ets_dop\dialogs\servicedlg\serviceviewdlg_autogen\mocs_compilation.cpp > CMakeFiles\serviceviewdlg.dir\serviceviewdlg_autogen\mocs_compilation.cpp.i

dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/serviceviewdlg_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serviceviewdlg.dir/serviceviewdlg_autogen/mocs_compilation.cpp.s"
	cd /d D:\IPS\configurator_ets_dop\dialogs\servicedlg && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\IPS\configurator_ets_dop\dialogs\servicedlg\serviceviewdlg_autogen\mocs_compilation.cpp -o CMakeFiles\serviceviewdlg.dir\serviceviewdlg_autogen\mocs_compilation.cpp.s

dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/serviceviewdlg.cpp.obj: dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/flags.make
dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/serviceviewdlg.cpp.obj: dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/includes_CXX.rsp
dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/serviceviewdlg.cpp.obj: dialogs/servicedlg/serviceviewdlg.cpp
dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/serviceviewdlg.cpp.obj: dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\IPS\configurator_ets_dop\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/serviceviewdlg.cpp.obj"
	cd /d D:\IPS\configurator_ets_dop\dialogs\servicedlg && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/serviceviewdlg.cpp.obj -MF CMakeFiles\serviceviewdlg.dir\serviceviewdlg.cpp.obj.d -o CMakeFiles\serviceviewdlg.dir\serviceviewdlg.cpp.obj -c D:\IPS\configurator_ets_dop\dialogs\servicedlg\serviceviewdlg.cpp

dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/serviceviewdlg.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serviceviewdlg.dir/serviceviewdlg.cpp.i"
	cd /d D:\IPS\configurator_ets_dop\dialogs\servicedlg && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\IPS\configurator_ets_dop\dialogs\servicedlg\serviceviewdlg.cpp > CMakeFiles\serviceviewdlg.dir\serviceviewdlg.cpp.i

dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/serviceviewdlg.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serviceviewdlg.dir/serviceviewdlg.cpp.s"
	cd /d D:\IPS\configurator_ets_dop\dialogs\servicedlg && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\IPS\configurator_ets_dop\dialogs\servicedlg\serviceviewdlg.cpp -o CMakeFiles\serviceviewdlg.dir\serviceviewdlg.cpp.s

# Object files for target serviceviewdlg
serviceviewdlg_OBJECTS = \
"CMakeFiles/serviceviewdlg.dir/serviceviewdlg_autogen/mocs_compilation.cpp.obj" \
"CMakeFiles/serviceviewdlg.dir/serviceviewdlg.cpp.obj"

# External object files for target serviceviewdlg
serviceviewdlg_EXTERNAL_OBJECTS =

bin_win10/libserviceviewdlg.dll: dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/serviceviewdlg_autogen/mocs_compilation.cpp.obj
bin_win10/libserviceviewdlg.dll: dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/serviceviewdlg.cpp.obj
bin_win10/libserviceviewdlg.dll: dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/build.make
bin_win10/libserviceviewdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Widgets.a
bin_win10/libserviceviewdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Gui.a
bin_win10/libserviceviewdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5SerialPort.a
bin_win10/libserviceviewdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Network.a
bin_win10/libserviceviewdlg.dll: dialogs/cryptodlg/libcryptodlg.dll.a
bin_win10/libserviceviewdlg.dll: dialogs/msgitemsmodel/libmsgtablemodel.dll.a
bin_win10/libserviceviewdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Core.a
bin_win10/libserviceviewdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Widgets.a
bin_win10/libserviceviewdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Gui.a
bin_win10/libserviceviewdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Test.a
bin_win10/libserviceviewdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Core.a
bin_win10/libserviceviewdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Widgets.a
bin_win10/libserviceviewdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Gui.a
bin_win10/libserviceviewdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Core.a
bin_win10/libserviceviewdlg.dll: dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/linklibs.rsp
bin_win10/libserviceviewdlg.dll: dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/objects1.rsp
bin_win10/libserviceviewdlg.dll: dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\IPS\configurator_ets_dop\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX shared library ..\..\bin_win10\libserviceviewdlg.dll"
	cd /d D:\IPS\configurator_ets_dop\dialogs\servicedlg && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\serviceviewdlg.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/build: bin_win10/libserviceviewdlg.dll
.PHONY : dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/build

dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/clean:
	cd /d D:\IPS\configurator_ets_dop\dialogs\servicedlg && $(CMAKE_COMMAND) -P CMakeFiles\serviceviewdlg.dir\cmake_clean.cmake
.PHONY : dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/clean

dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/depend: dialogs/servicedlg/ui_serviceviewdlg.h
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\IPS\configurator_ets_dop D:\IPS\configurator_ets_dop\dialogs\servicedlg D:\IPS\configurator_ets_dop D:\IPS\configurator_ets_dop\dialogs\servicedlg D:\IPS\configurator_ets_dop\dialogs\servicedlg\CMakeFiles\serviceviewdlg.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : dialogs/servicedlg/CMakeFiles/serviceviewdlg.dir/depend

