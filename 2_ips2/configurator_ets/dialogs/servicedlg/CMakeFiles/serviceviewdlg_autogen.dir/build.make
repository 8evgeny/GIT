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

# Utility rule file for serviceviewdlg_autogen.

# Include any custom commands dependencies for this target.
include dialogs/servicedlg/CMakeFiles/serviceviewdlg_autogen.dir/compiler_depend.make

# Include the progress variables for this target.
include dialogs/servicedlg/CMakeFiles/serviceviewdlg_autogen.dir/progress.make

dialogs/servicedlg/CMakeFiles/serviceviewdlg_autogen:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=D:\IPS\configurator_ets_dop\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC and UIC for target serviceviewdlg"
	cd /d D:\IPS\configurator_ets_dop\dialogs\servicedlg && "C:\Program Files\CMake\bin\cmake.exe" -E cmake_autogen D:/IPS/configurator_ets_dop/dialogs/servicedlg/CMakeFiles/serviceviewdlg_autogen.dir/AutogenInfo.json ""

serviceviewdlg_autogen: dialogs/servicedlg/CMakeFiles/serviceviewdlg_autogen
serviceviewdlg_autogen: dialogs/servicedlg/CMakeFiles/serviceviewdlg_autogen.dir/build.make
.PHONY : serviceviewdlg_autogen

# Rule to build all files generated by this target.
dialogs/servicedlg/CMakeFiles/serviceviewdlg_autogen.dir/build: serviceviewdlg_autogen
.PHONY : dialogs/servicedlg/CMakeFiles/serviceviewdlg_autogen.dir/build

dialogs/servicedlg/CMakeFiles/serviceviewdlg_autogen.dir/clean:
	cd /d D:\IPS\configurator_ets_dop\dialogs\servicedlg && $(CMAKE_COMMAND) -P CMakeFiles\serviceviewdlg_autogen.dir\cmake_clean.cmake
.PHONY : dialogs/servicedlg/CMakeFiles/serviceviewdlg_autogen.dir/clean

dialogs/servicedlg/CMakeFiles/serviceviewdlg_autogen.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\IPS\configurator_ets_dop D:\IPS\configurator_ets_dop\dialogs\servicedlg D:\IPS\configurator_ets_dop D:\IPS\configurator_ets_dop\dialogs\servicedlg D:\IPS\configurator_ets_dop\dialogs\servicedlg\CMakeFiles\serviceviewdlg_autogen.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : dialogs/servicedlg/CMakeFiles/serviceviewdlg_autogen.dir/depend

