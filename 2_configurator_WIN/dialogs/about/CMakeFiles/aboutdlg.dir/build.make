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
include dialogs/about/CMakeFiles/aboutdlg.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include dialogs/about/CMakeFiles/aboutdlg.dir/compiler_depend.make

# Include the progress variables for this target.
include dialogs/about/CMakeFiles/aboutdlg.dir/progress.make

# Include the compile flags for this target's objects.
include dialogs/about/CMakeFiles/aboutdlg.dir/flags.make

dialogs/about/ui_about.h: dialogs/about/about.ui
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=D:\IPS\configurator_ets_dop\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating ui_about.h"
	cd /d D:\IPS\configurator_ets_dop\dialogs\about && C:\QT5.15MinGW810\Qt515\bin\uic.exe -o D:/IPS/configurator_ets_dop/dialogs/about/ui_about.h D:/IPS/configurator_ets_dop/dialogs/about/about.ui

dialogs/about/CMakeFiles/aboutdlg.dir/aboutdlg_autogen/mocs_compilation.cpp.obj: dialogs/about/CMakeFiles/aboutdlg.dir/flags.make
dialogs/about/CMakeFiles/aboutdlg.dir/aboutdlg_autogen/mocs_compilation.cpp.obj: dialogs/about/CMakeFiles/aboutdlg.dir/includes_CXX.rsp
dialogs/about/CMakeFiles/aboutdlg.dir/aboutdlg_autogen/mocs_compilation.cpp.obj: dialogs/about/aboutdlg_autogen/mocs_compilation.cpp
dialogs/about/CMakeFiles/aboutdlg.dir/aboutdlg_autogen/mocs_compilation.cpp.obj: dialogs/about/CMakeFiles/aboutdlg.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\IPS\configurator_ets_dop\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object dialogs/about/CMakeFiles/aboutdlg.dir/aboutdlg_autogen/mocs_compilation.cpp.obj"
	cd /d D:\IPS\configurator_ets_dop\dialogs\about && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT dialogs/about/CMakeFiles/aboutdlg.dir/aboutdlg_autogen/mocs_compilation.cpp.obj -MF CMakeFiles\aboutdlg.dir\aboutdlg_autogen\mocs_compilation.cpp.obj.d -o CMakeFiles\aboutdlg.dir\aboutdlg_autogen\mocs_compilation.cpp.obj -c D:\IPS\configurator_ets_dop\dialogs\about\aboutdlg_autogen\mocs_compilation.cpp

dialogs/about/CMakeFiles/aboutdlg.dir/aboutdlg_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/aboutdlg.dir/aboutdlg_autogen/mocs_compilation.cpp.i"
	cd /d D:\IPS\configurator_ets_dop\dialogs\about && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\IPS\configurator_ets_dop\dialogs\about\aboutdlg_autogen\mocs_compilation.cpp > CMakeFiles\aboutdlg.dir\aboutdlg_autogen\mocs_compilation.cpp.i

dialogs/about/CMakeFiles/aboutdlg.dir/aboutdlg_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/aboutdlg.dir/aboutdlg_autogen/mocs_compilation.cpp.s"
	cd /d D:\IPS\configurator_ets_dop\dialogs\about && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\IPS\configurator_ets_dop\dialogs\about\aboutdlg_autogen\mocs_compilation.cpp -o CMakeFiles\aboutdlg.dir\aboutdlg_autogen\mocs_compilation.cpp.s

dialogs/about/CMakeFiles/aboutdlg.dir/about.cpp.obj: dialogs/about/CMakeFiles/aboutdlg.dir/flags.make
dialogs/about/CMakeFiles/aboutdlg.dir/about.cpp.obj: dialogs/about/CMakeFiles/aboutdlg.dir/includes_CXX.rsp
dialogs/about/CMakeFiles/aboutdlg.dir/about.cpp.obj: dialogs/about/about.cpp
dialogs/about/CMakeFiles/aboutdlg.dir/about.cpp.obj: dialogs/about/CMakeFiles/aboutdlg.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\IPS\configurator_ets_dop\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object dialogs/about/CMakeFiles/aboutdlg.dir/about.cpp.obj"
	cd /d D:\IPS\configurator_ets_dop\dialogs\about && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT dialogs/about/CMakeFiles/aboutdlg.dir/about.cpp.obj -MF CMakeFiles\aboutdlg.dir\about.cpp.obj.d -o CMakeFiles\aboutdlg.dir\about.cpp.obj -c D:\IPS\configurator_ets_dop\dialogs\about\about.cpp

dialogs/about/CMakeFiles/aboutdlg.dir/about.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/aboutdlg.dir/about.cpp.i"
	cd /d D:\IPS\configurator_ets_dop\dialogs\about && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\IPS\configurator_ets_dop\dialogs\about\about.cpp > CMakeFiles\aboutdlg.dir\about.cpp.i

dialogs/about/CMakeFiles/aboutdlg.dir/about.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/aboutdlg.dir/about.cpp.s"
	cd /d D:\IPS\configurator_ets_dop\dialogs\about && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\IPS\configurator_ets_dop\dialogs\about\about.cpp -o CMakeFiles\aboutdlg.dir\about.cpp.s

# Object files for target aboutdlg
aboutdlg_OBJECTS = \
"CMakeFiles/aboutdlg.dir/aboutdlg_autogen/mocs_compilation.cpp.obj" \
"CMakeFiles/aboutdlg.dir/about.cpp.obj"

# External object files for target aboutdlg
aboutdlg_EXTERNAL_OBJECTS =

bin_win10/libaboutdlg.dll: dialogs/about/CMakeFiles/aboutdlg.dir/aboutdlg_autogen/mocs_compilation.cpp.obj
bin_win10/libaboutdlg.dll: dialogs/about/CMakeFiles/aboutdlg.dir/about.cpp.obj
bin_win10/libaboutdlg.dll: dialogs/about/CMakeFiles/aboutdlg.dir/build.make
bin_win10/libaboutdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Widgets.a
bin_win10/libaboutdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Gui.a
bin_win10/libaboutdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Core.a
bin_win10/libaboutdlg.dll: dialogs/about/CMakeFiles/aboutdlg.dir/linklibs.rsp
bin_win10/libaboutdlg.dll: dialogs/about/CMakeFiles/aboutdlg.dir/objects1.rsp
bin_win10/libaboutdlg.dll: dialogs/about/CMakeFiles/aboutdlg.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\IPS\configurator_ets_dop\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX shared library ..\..\bin_win10\libaboutdlg.dll"
	cd /d D:\IPS\configurator_ets_dop\dialogs\about && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\aboutdlg.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
dialogs/about/CMakeFiles/aboutdlg.dir/build: bin_win10/libaboutdlg.dll
.PHONY : dialogs/about/CMakeFiles/aboutdlg.dir/build

dialogs/about/CMakeFiles/aboutdlg.dir/clean:
	cd /d D:\IPS\configurator_ets_dop\dialogs\about && $(CMAKE_COMMAND) -P CMakeFiles\aboutdlg.dir\cmake_clean.cmake
.PHONY : dialogs/about/CMakeFiles/aboutdlg.dir/clean

dialogs/about/CMakeFiles/aboutdlg.dir/depend: dialogs/about/ui_about.h
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\IPS\configurator_ets_dop D:\IPS\configurator_ets_dop\dialogs\about D:\IPS\configurator_ets_dop D:\IPS\configurator_ets_dop\dialogs\about D:\IPS\configurator_ets_dop\dialogs\about\CMakeFiles\aboutdlg.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : dialogs/about/CMakeFiles/aboutdlg.dir/depend

