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
include dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/compiler_depend.make

# Include the progress variables for this target.
include dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/progress.make

# Include the compile flags for this target's objects.
include dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/flags.make

dialogs/setupconnectionsdlg/ui_setupconnections.h: dialogs/setupconnectionsdlg/setupconnections.ui
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=D:\IPS\configurator_ets_dop\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating ui_setupconnections.h"
	cd /d D:\IPS\configurator_ets_dop\dialogs\setupconnectionsdlg && C:\QT5.15MinGW810\Qt515\bin\uic.exe -o D:/IPS/configurator_ets_dop/dialogs/setupconnectionsdlg/ui_setupconnections.h D:/IPS/configurator_ets_dop/dialogs/setupconnectionsdlg/setupconnections.ui

dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/setupconnectionsdlg_autogen/mocs_compilation.cpp.obj: dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/flags.make
dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/setupconnectionsdlg_autogen/mocs_compilation.cpp.obj: dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/includes_CXX.rsp
dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/setupconnectionsdlg_autogen/mocs_compilation.cpp.obj: dialogs/setupconnectionsdlg/setupconnectionsdlg_autogen/mocs_compilation.cpp
dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/setupconnectionsdlg_autogen/mocs_compilation.cpp.obj: dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\IPS\configurator_ets_dop\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/setupconnectionsdlg_autogen/mocs_compilation.cpp.obj"
	cd /d D:\IPS\configurator_ets_dop\dialogs\setupconnectionsdlg && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/setupconnectionsdlg_autogen/mocs_compilation.cpp.obj -MF CMakeFiles\setupconnectionsdlg.dir\setupconnectionsdlg_autogen\mocs_compilation.cpp.obj.d -o CMakeFiles\setupconnectionsdlg.dir\setupconnectionsdlg_autogen\mocs_compilation.cpp.obj -c D:\IPS\configurator_ets_dop\dialogs\setupconnectionsdlg\setupconnectionsdlg_autogen\mocs_compilation.cpp

dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/setupconnectionsdlg_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/setupconnectionsdlg.dir/setupconnectionsdlg_autogen/mocs_compilation.cpp.i"
	cd /d D:\IPS\configurator_ets_dop\dialogs\setupconnectionsdlg && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\IPS\configurator_ets_dop\dialogs\setupconnectionsdlg\setupconnectionsdlg_autogen\mocs_compilation.cpp > CMakeFiles\setupconnectionsdlg.dir\setupconnectionsdlg_autogen\mocs_compilation.cpp.i

dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/setupconnectionsdlg_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/setupconnectionsdlg.dir/setupconnectionsdlg_autogen/mocs_compilation.cpp.s"
	cd /d D:\IPS\configurator_ets_dop\dialogs\setupconnectionsdlg && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\IPS\configurator_ets_dop\dialogs\setupconnectionsdlg\setupconnectionsdlg_autogen\mocs_compilation.cpp -o CMakeFiles\setupconnectionsdlg.dir\setupconnectionsdlg_autogen\mocs_compilation.cpp.s

dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/setupconnections.cpp.obj: dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/flags.make
dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/setupconnections.cpp.obj: dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/includes_CXX.rsp
dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/setupconnections.cpp.obj: dialogs/setupconnectionsdlg/setupconnections.cpp
dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/setupconnections.cpp.obj: dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\IPS\configurator_ets_dop\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/setupconnections.cpp.obj"
	cd /d D:\IPS\configurator_ets_dop\dialogs\setupconnectionsdlg && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/setupconnections.cpp.obj -MF CMakeFiles\setupconnectionsdlg.dir\setupconnections.cpp.obj.d -o CMakeFiles\setupconnectionsdlg.dir\setupconnections.cpp.obj -c D:\IPS\configurator_ets_dop\dialogs\setupconnectionsdlg\setupconnections.cpp

dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/setupconnections.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/setupconnectionsdlg.dir/setupconnections.cpp.i"
	cd /d D:\IPS\configurator_ets_dop\dialogs\setupconnectionsdlg && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\IPS\configurator_ets_dop\dialogs\setupconnectionsdlg\setupconnections.cpp > CMakeFiles\setupconnectionsdlg.dir\setupconnections.cpp.i

dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/setupconnections.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/setupconnectionsdlg.dir/setupconnections.cpp.s"
	cd /d D:\IPS\configurator_ets_dop\dialogs\setupconnectionsdlg && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\IPS\configurator_ets_dop\dialogs\setupconnectionsdlg\setupconnections.cpp -o CMakeFiles\setupconnectionsdlg.dir\setupconnections.cpp.s

# Object files for target setupconnectionsdlg
setupconnectionsdlg_OBJECTS = \
"CMakeFiles/setupconnectionsdlg.dir/setupconnectionsdlg_autogen/mocs_compilation.cpp.obj" \
"CMakeFiles/setupconnectionsdlg.dir/setupconnections.cpp.obj"

# External object files for target setupconnectionsdlg
setupconnectionsdlg_EXTERNAL_OBJECTS =

bin_win10/libsetupconnectionsdlg.dll: dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/setupconnectionsdlg_autogen/mocs_compilation.cpp.obj
bin_win10/libsetupconnectionsdlg.dll: dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/setupconnections.cpp.obj
bin_win10/libsetupconnectionsdlg.dll: dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/build.make
bin_win10/libsetupconnectionsdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Widgets.a
bin_win10/libsetupconnectionsdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Gui.a
bin_win10/libsetupconnectionsdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5SerialPort.a
bin_win10/libsetupconnectionsdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Network.a
bin_win10/libsetupconnectionsdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Core.a
bin_win10/libsetupconnectionsdlg.dll: dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/linklibs.rsp
bin_win10/libsetupconnectionsdlg.dll: dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/objects1.rsp
bin_win10/libsetupconnectionsdlg.dll: dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\IPS\configurator_ets_dop\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX shared library ..\..\bin_win10\libsetupconnectionsdlg.dll"
	cd /d D:\IPS\configurator_ets_dop\dialogs\setupconnectionsdlg && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\setupconnectionsdlg.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/build: bin_win10/libsetupconnectionsdlg.dll
.PHONY : dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/build

dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/clean:
	cd /d D:\IPS\configurator_ets_dop\dialogs\setupconnectionsdlg && $(CMAKE_COMMAND) -P CMakeFiles\setupconnectionsdlg.dir\cmake_clean.cmake
.PHONY : dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/clean

dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/depend: dialogs/setupconnectionsdlg/ui_setupconnections.h
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\IPS\configurator_ets_dop D:\IPS\configurator_ets_dop\dialogs\setupconnectionsdlg D:\IPS\configurator_ets_dop D:\IPS\configurator_ets_dop\dialogs\setupconnectionsdlg D:\IPS\configurator_ets_dop\dialogs\setupconnectionsdlg\CMakeFiles\setupconnectionsdlg.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : dialogs/setupconnectionsdlg/CMakeFiles/setupconnectionsdlg.dir/depend

