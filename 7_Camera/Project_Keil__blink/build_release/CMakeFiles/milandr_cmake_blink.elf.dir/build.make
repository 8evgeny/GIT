# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.21

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/evg/Qt/Tools/CMake/bin/cmake

# The command to remove a file.
RM = /home/evg/Qt/Tools/CMake/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/evg/SOFT/Github/GIT/7_Camera/Project_Keil__blink

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/evg/SOFT/Github/GIT/7_Camera/Project_Keil__blink/build_release

# Include any dependencies generated for this target.
include CMakeFiles/milandr_cmake_blink.elf.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/milandr_cmake_blink.elf.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/milandr_cmake_blink.elf.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/milandr_cmake_blink.elf.dir/flags.make

CMakeFiles/milandr_cmake_blink.elf.dir/UserCode/main.cpp.o: CMakeFiles/milandr_cmake_blink.elf.dir/flags.make
CMakeFiles/milandr_cmake_blink.elf.dir/UserCode/main.cpp.o: ../UserCode/main.cpp
CMakeFiles/milandr_cmake_blink.elf.dir/UserCode/main.cpp.o: CMakeFiles/milandr_cmake_blink.elf.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/evg/SOFT/Github/GIT/7_Camera/Project_Keil__blink/build_release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/milandr_cmake_blink.elf.dir/UserCode/main.cpp.o"
	/usr/bin/arm-none-eabi-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/milandr_cmake_blink.elf.dir/UserCode/main.cpp.o -MF CMakeFiles/milandr_cmake_blink.elf.dir/UserCode/main.cpp.o.d -o CMakeFiles/milandr_cmake_blink.elf.dir/UserCode/main.cpp.o -c /home/evg/SOFT/Github/GIT/7_Camera/Project_Keil__blink/UserCode/main.cpp

CMakeFiles/milandr_cmake_blink.elf.dir/UserCode/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/milandr_cmake_blink.elf.dir/UserCode/main.cpp.i"
	/usr/bin/arm-none-eabi-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/evg/SOFT/Github/GIT/7_Camera/Project_Keil__blink/UserCode/main.cpp > CMakeFiles/milandr_cmake_blink.elf.dir/UserCode/main.cpp.i

CMakeFiles/milandr_cmake_blink.elf.dir/UserCode/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/milandr_cmake_blink.elf.dir/UserCode/main.cpp.s"
	/usr/bin/arm-none-eabi-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/evg/SOFT/Github/GIT/7_Camera/Project_Keil__blink/UserCode/main.cpp -o CMakeFiles/milandr_cmake_blink.elf.dir/UserCode/main.cpp.s

# Object files for target milandr_cmake_blink.elf
milandr_cmake_blink_elf_OBJECTS = \
"CMakeFiles/milandr_cmake_blink.elf.dir/UserCode/main.cpp.o"

# External object files for target milandr_cmake_blink.elf
milandr_cmake_blink_elf_EXTERNAL_OBJECTS =

milandr_cmake_blink.elf: CMakeFiles/milandr_cmake_blink.elf.dir/UserCode/main.cpp.o
milandr_cmake_blink.elf: CMakeFiles/milandr_cmake_blink.elf.dir/build.make
milandr_cmake_blink.elf: libCMSIS.a
milandr_cmake_blink.elf: CMakeFiles/milandr_cmake_blink.elf.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/evg/SOFT/Github/GIT/7_Camera/Project_Keil__blink/build_release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable milandr_cmake_blink.elf"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/milandr_cmake_blink.elf.dir/link.txt --verbose=$(VERBOSE)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Building /home/evg/SOFT/Github/GIT/7_Camera/Project_Keil__blink/build_release/milandr_cmake_blink.hex "
	/usr/bin/arm-none-eabi-objcopy -Oihex /home/evg/SOFT/Github/GIT/7_Camera/Project_Keil__blink/build_release/milandr_cmake_blink.elf /home/evg/SOFT/Github/GIT/7_Camera/Project_Keil__blink/build_release/milandr_cmake_blink.hex
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Building /home/evg/SOFT/Github/GIT/7_Camera/Project_Keil__blink/build_release/milandr_cmake_blink.bin "
	/usr/bin/arm-none-eabi-objcopy -O binary /home/evg/SOFT/Github/GIT/7_Camera/Project_Keil__blink/build_release/milandr_cmake_blink.elf /home/evg/SOFT/Github/GIT/7_Camera/Project_Keil__blink/build_release/milandr_cmake_blink.bin

# Rule to build all files generated by this target.
CMakeFiles/milandr_cmake_blink.elf.dir/build: milandr_cmake_blink.elf
.PHONY : CMakeFiles/milandr_cmake_blink.elf.dir/build

CMakeFiles/milandr_cmake_blink.elf.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/milandr_cmake_blink.elf.dir/cmake_clean.cmake
.PHONY : CMakeFiles/milandr_cmake_blink.elf.dir/clean

CMakeFiles/milandr_cmake_blink.elf.dir/depend:
	cd /home/evg/SOFT/Github/GIT/7_Camera/Project_Keil__blink/build_release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/evg/SOFT/Github/GIT/7_Camera/Project_Keil__blink /home/evg/SOFT/Github/GIT/7_Camera/Project_Keil__blink /home/evg/SOFT/Github/GIT/7_Camera/Project_Keil__blink/build_release /home/evg/SOFT/Github/GIT/7_Camera/Project_Keil__blink/build_release /home/evg/SOFT/Github/GIT/7_Camera/Project_Keil__blink/build_release/CMakeFiles/milandr_cmake_blink.elf.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/milandr_cmake_blink.elf.dir/depend

