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
include dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/compiler_depend.make

# Include the progress variables for this target.
include dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/progress.make

# Include the compile flags for this target's objects.
include dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/flags.make

dialogs/instruments/test/ui_ipstest.h: dialogs/instruments/test/ipstest.ui
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=D:\IPS\configurator_ets_dop\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating ui_ipstest.h"
	cd /d D:\IPS\configurator_ets_dop\dialogs\instruments\test && C:\QT5.15MinGW810\Qt515\bin\uic.exe -o D:/IPS/configurator_ets_dop/dialogs/instruments/test/ui_ipstest.h D:/IPS/configurator_ets_dop/dialogs/instruments/test/ipstest.ui

dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/ipstestdlg_autogen/mocs_compilation.cpp.obj: dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/flags.make
dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/ipstestdlg_autogen/mocs_compilation.cpp.obj: dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/includes_CXX.rsp
dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/ipstestdlg_autogen/mocs_compilation.cpp.obj: dialogs/instruments/test/ipstestdlg_autogen/mocs_compilation.cpp
dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/ipstestdlg_autogen/mocs_compilation.cpp.obj: dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\IPS\configurator_ets_dop\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/ipstestdlg_autogen/mocs_compilation.cpp.obj"
	cd /d D:\IPS\configurator_ets_dop\dialogs\instruments\test && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/ipstestdlg_autogen/mocs_compilation.cpp.obj -MF CMakeFiles\ipstestdlg.dir\ipstestdlg_autogen\mocs_compilation.cpp.obj.d -o CMakeFiles\ipstestdlg.dir\ipstestdlg_autogen\mocs_compilation.cpp.obj -c D:\IPS\configurator_ets_dop\dialogs\instruments\test\ipstestdlg_autogen\mocs_compilation.cpp

dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/ipstestdlg_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ipstestdlg.dir/ipstestdlg_autogen/mocs_compilation.cpp.i"
	cd /d D:\IPS\configurator_ets_dop\dialogs\instruments\test && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\IPS\configurator_ets_dop\dialogs\instruments\test\ipstestdlg_autogen\mocs_compilation.cpp > CMakeFiles\ipstestdlg.dir\ipstestdlg_autogen\mocs_compilation.cpp.i

dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/ipstestdlg_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ipstestdlg.dir/ipstestdlg_autogen/mocs_compilation.cpp.s"
	cd /d D:\IPS\configurator_ets_dop\dialogs\instruments\test && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\IPS\configurator_ets_dop\dialogs\instruments\test\ipstestdlg_autogen\mocs_compilation.cpp -o CMakeFiles\ipstestdlg.dir\ipstestdlg_autogen\mocs_compilation.cpp.s

dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/ipstest.cpp.obj: dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/flags.make
dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/ipstest.cpp.obj: dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/includes_CXX.rsp
dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/ipstest.cpp.obj: dialogs/instruments/test/ipstest.cpp
dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/ipstest.cpp.obj: dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\IPS\configurator_ets_dop\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/ipstest.cpp.obj"
	cd /d D:\IPS\configurator_ets_dop\dialogs\instruments\test && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/ipstest.cpp.obj -MF CMakeFiles\ipstestdlg.dir\ipstest.cpp.obj.d -o CMakeFiles\ipstestdlg.dir\ipstest.cpp.obj -c D:\IPS\configurator_ets_dop\dialogs\instruments\test\ipstest.cpp

dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/ipstest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ipstestdlg.dir/ipstest.cpp.i"
	cd /d D:\IPS\configurator_ets_dop\dialogs\instruments\test && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\IPS\configurator_ets_dop\dialogs\instruments\test\ipstest.cpp > CMakeFiles\ipstestdlg.dir\ipstest.cpp.i

dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/ipstest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ipstestdlg.dir/ipstest.cpp.s"
	cd /d D:\IPS\configurator_ets_dop\dialogs\instruments\test && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\IPS\configurator_ets_dop\dialogs\instruments\test\ipstest.cpp -o CMakeFiles\ipstestdlg.dir\ipstest.cpp.s

# Object files for target ipstestdlg
ipstestdlg_OBJECTS = \
"CMakeFiles/ipstestdlg.dir/ipstestdlg_autogen/mocs_compilation.cpp.obj" \
"CMakeFiles/ipstestdlg.dir/ipstest.cpp.obj"

# External object files for target ipstestdlg
ipstestdlg_EXTERNAL_OBJECTS =

bin_win10/libipstestdlg.dll: dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/ipstestdlg_autogen/mocs_compilation.cpp.obj
bin_win10/libipstestdlg.dll: dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/ipstest.cpp.obj
bin_win10/libipstestdlg.dll: dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/build.make
bin_win10/libipstestdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Widgets.a
bin_win10/libipstestdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Gui.a
bin_win10/libipstestdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Core.a
bin_win10/libipstestdlg.dll: dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/linklibs.rsp
bin_win10/libipstestdlg.dll: dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/objects1.rsp
bin_win10/libipstestdlg.dll: dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\IPS\configurator_ets_dop\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX shared library ..\..\..\bin_win10\libipstestdlg.dll"
	cd /d D:\IPS\configurator_ets_dop\dialogs\instruments\test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\ipstestdlg.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/build: bin_win10/libipstestdlg.dll
.PHONY : dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/build

dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/clean:
	cd /d D:\IPS\configurator_ets_dop\dialogs\instruments\test && $(CMAKE_COMMAND) -P CMakeFiles\ipstestdlg.dir\cmake_clean.cmake
.PHONY : dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/clean

dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/depend: dialogs/instruments/test/ui_ipstest.h
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\IPS\configurator_ets_dop D:\IPS\configurator_ets_dop\dialogs\instruments\test D:\IPS\configurator_ets_dop D:\IPS\configurator_ets_dop\dialogs\instruments\test D:\IPS\configurator_ets_dop\dialogs\instruments\test\CMakeFiles\ipstestdlg.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : dialogs/instruments/test/CMakeFiles/ipstestdlg.dir/depend

