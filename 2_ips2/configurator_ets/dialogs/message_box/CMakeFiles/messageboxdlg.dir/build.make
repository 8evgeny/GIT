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
include dialogs/message_box/CMakeFiles/messageboxdlg.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include dialogs/message_box/CMakeFiles/messageboxdlg.dir/compiler_depend.make

# Include the progress variables for this target.
include dialogs/message_box/CMakeFiles/messageboxdlg.dir/progress.make

# Include the compile flags for this target's objects.
include dialogs/message_box/CMakeFiles/messageboxdlg.dir/flags.make

dialogs/message_box/ui_message_box.h: dialogs/message_box/message_box.ui
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=D:\IPS\configurator_ets_dop\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating ui_message_box.h"
	cd /d D:\IPS\configurator_ets_dop\dialogs\message_box && C:\QT5.15MinGW810\Qt515\bin\uic.exe -o D:/IPS/configurator_ets_dop/dialogs/message_box/ui_message_box.h D:/IPS/configurator_ets_dop/dialogs/message_box/message_box.ui

dialogs/message_box/CMakeFiles/messageboxdlg.dir/messageboxdlg_autogen/mocs_compilation.cpp.obj: dialogs/message_box/CMakeFiles/messageboxdlg.dir/flags.make
dialogs/message_box/CMakeFiles/messageboxdlg.dir/messageboxdlg_autogen/mocs_compilation.cpp.obj: dialogs/message_box/CMakeFiles/messageboxdlg.dir/includes_CXX.rsp
dialogs/message_box/CMakeFiles/messageboxdlg.dir/messageboxdlg_autogen/mocs_compilation.cpp.obj: dialogs/message_box/messageboxdlg_autogen/mocs_compilation.cpp
dialogs/message_box/CMakeFiles/messageboxdlg.dir/messageboxdlg_autogen/mocs_compilation.cpp.obj: dialogs/message_box/CMakeFiles/messageboxdlg.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\IPS\configurator_ets_dop\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object dialogs/message_box/CMakeFiles/messageboxdlg.dir/messageboxdlg_autogen/mocs_compilation.cpp.obj"
	cd /d D:\IPS\configurator_ets_dop\dialogs\message_box && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT dialogs/message_box/CMakeFiles/messageboxdlg.dir/messageboxdlg_autogen/mocs_compilation.cpp.obj -MF CMakeFiles\messageboxdlg.dir\messageboxdlg_autogen\mocs_compilation.cpp.obj.d -o CMakeFiles\messageboxdlg.dir\messageboxdlg_autogen\mocs_compilation.cpp.obj -c D:\IPS\configurator_ets_dop\dialogs\message_box\messageboxdlg_autogen\mocs_compilation.cpp

dialogs/message_box/CMakeFiles/messageboxdlg.dir/messageboxdlg_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/messageboxdlg.dir/messageboxdlg_autogen/mocs_compilation.cpp.i"
	cd /d D:\IPS\configurator_ets_dop\dialogs\message_box && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\IPS\configurator_ets_dop\dialogs\message_box\messageboxdlg_autogen\mocs_compilation.cpp > CMakeFiles\messageboxdlg.dir\messageboxdlg_autogen\mocs_compilation.cpp.i

dialogs/message_box/CMakeFiles/messageboxdlg.dir/messageboxdlg_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/messageboxdlg.dir/messageboxdlg_autogen/mocs_compilation.cpp.s"
	cd /d D:\IPS\configurator_ets_dop\dialogs\message_box && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\IPS\configurator_ets_dop\dialogs\message_box\messageboxdlg_autogen\mocs_compilation.cpp -o CMakeFiles\messageboxdlg.dir\messageboxdlg_autogen\mocs_compilation.cpp.s

dialogs/message_box/CMakeFiles/messageboxdlg.dir/message_box.cpp.obj: dialogs/message_box/CMakeFiles/messageboxdlg.dir/flags.make
dialogs/message_box/CMakeFiles/messageboxdlg.dir/message_box.cpp.obj: dialogs/message_box/CMakeFiles/messageboxdlg.dir/includes_CXX.rsp
dialogs/message_box/CMakeFiles/messageboxdlg.dir/message_box.cpp.obj: dialogs/message_box/message_box.cpp
dialogs/message_box/CMakeFiles/messageboxdlg.dir/message_box.cpp.obj: dialogs/message_box/CMakeFiles/messageboxdlg.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\IPS\configurator_ets_dop\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object dialogs/message_box/CMakeFiles/messageboxdlg.dir/message_box.cpp.obj"
	cd /d D:\IPS\configurator_ets_dop\dialogs\message_box && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT dialogs/message_box/CMakeFiles/messageboxdlg.dir/message_box.cpp.obj -MF CMakeFiles\messageboxdlg.dir\message_box.cpp.obj.d -o CMakeFiles\messageboxdlg.dir\message_box.cpp.obj -c D:\IPS\configurator_ets_dop\dialogs\message_box\message_box.cpp

dialogs/message_box/CMakeFiles/messageboxdlg.dir/message_box.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/messageboxdlg.dir/message_box.cpp.i"
	cd /d D:\IPS\configurator_ets_dop\dialogs\message_box && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\IPS\configurator_ets_dop\dialogs\message_box\message_box.cpp > CMakeFiles\messageboxdlg.dir\message_box.cpp.i

dialogs/message_box/CMakeFiles/messageboxdlg.dir/message_box.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/messageboxdlg.dir/message_box.cpp.s"
	cd /d D:\IPS\configurator_ets_dop\dialogs\message_box && C:\QT5.15MinGW810\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\IPS\configurator_ets_dop\dialogs\message_box\message_box.cpp -o CMakeFiles\messageboxdlg.dir\message_box.cpp.s

# Object files for target messageboxdlg
messageboxdlg_OBJECTS = \
"CMakeFiles/messageboxdlg.dir/messageboxdlg_autogen/mocs_compilation.cpp.obj" \
"CMakeFiles/messageboxdlg.dir/message_box.cpp.obj"

# External object files for target messageboxdlg
messageboxdlg_EXTERNAL_OBJECTS =

bin_win10/libmessageboxdlg.dll: dialogs/message_box/CMakeFiles/messageboxdlg.dir/messageboxdlg_autogen/mocs_compilation.cpp.obj
bin_win10/libmessageboxdlg.dll: dialogs/message_box/CMakeFiles/messageboxdlg.dir/message_box.cpp.obj
bin_win10/libmessageboxdlg.dll: dialogs/message_box/CMakeFiles/messageboxdlg.dir/build.make
bin_win10/libmessageboxdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Widgets.a
bin_win10/libmessageboxdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Gui.a
bin_win10/libmessageboxdlg.dll: C:/QT5.15MinGW810/Qt515/lib/libQt5Core.a
bin_win10/libmessageboxdlg.dll: dialogs/message_box/CMakeFiles/messageboxdlg.dir/linklibs.rsp
bin_win10/libmessageboxdlg.dll: dialogs/message_box/CMakeFiles/messageboxdlg.dir/objects1.rsp
bin_win10/libmessageboxdlg.dll: dialogs/message_box/CMakeFiles/messageboxdlg.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\IPS\configurator_ets_dop\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX shared library ..\..\bin_win10\libmessageboxdlg.dll"
	cd /d D:\IPS\configurator_ets_dop\dialogs\message_box && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\messageboxdlg.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
dialogs/message_box/CMakeFiles/messageboxdlg.dir/build: bin_win10/libmessageboxdlg.dll
.PHONY : dialogs/message_box/CMakeFiles/messageboxdlg.dir/build

dialogs/message_box/CMakeFiles/messageboxdlg.dir/clean:
	cd /d D:\IPS\configurator_ets_dop\dialogs\message_box && $(CMAKE_COMMAND) -P CMakeFiles\messageboxdlg.dir\cmake_clean.cmake
.PHONY : dialogs/message_box/CMakeFiles/messageboxdlg.dir/clean

dialogs/message_box/CMakeFiles/messageboxdlg.dir/depend: dialogs/message_box/ui_message_box.h
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\IPS\configurator_ets_dop D:\IPS\configurator_ets_dop\dialogs\message_box D:\IPS\configurator_ets_dop D:\IPS\configurator_ets_dop\dialogs\message_box D:\IPS\configurator_ets_dop\dialogs\message_box\CMakeFiles\messageboxdlg.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : dialogs/message_box/CMakeFiles/messageboxdlg.dir/depend

