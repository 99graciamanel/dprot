# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_COMMAND = /snap/clion/169/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/169/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/manel/CLionProjects/dprot/dprot_pract2_4

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/manel/CLionProjects/dprot/dprot_pract2_4/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/dprot_pract2.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/dprot_pract2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/dprot_pract2.dir/flags.make

CMakeFiles/dprot_pract2.dir/main.c.o: CMakeFiles/dprot_pract2.dir/flags.make
CMakeFiles/dprot_pract2.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/manel/CLionProjects/dprot/dprot_pract2_4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/dprot_pract2.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/dprot_pract2.dir/main.c.o -c /home/manel/CLionProjects/dprot/dprot_pract2_4/main.c

CMakeFiles/dprot_pract2.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dprot_pract2.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/manel/CLionProjects/dprot/dprot_pract2_4/main.c > CMakeFiles/dprot_pract2.dir/main.c.i

CMakeFiles/dprot_pract2.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dprot_pract2.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/manel/CLionProjects/dprot/dprot_pract2_4/main.c -o CMakeFiles/dprot_pract2.dir/main.c.s

# Object files for target dprot_pract2
dprot_pract2_OBJECTS = \
"CMakeFiles/dprot_pract2.dir/main.c.o"

# External object files for target dprot_pract2
dprot_pract2_EXTERNAL_OBJECTS =

dprot_pract2: CMakeFiles/dprot_pract2.dir/main.c.o
dprot_pract2: CMakeFiles/dprot_pract2.dir/build.make
dprot_pract2: /usr/lib/x86_64-linux-gnu/libssl.so
dprot_pract2: /usr/lib/x86_64-linux-gnu/libcrypto.so
dprot_pract2: CMakeFiles/dprot_pract2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/manel/CLionProjects/dprot/dprot_pract2_4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable dprot_pract2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/dprot_pract2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/dprot_pract2.dir/build: dprot_pract2
.PHONY : CMakeFiles/dprot_pract2.dir/build

CMakeFiles/dprot_pract2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/dprot_pract2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/dprot_pract2.dir/clean

CMakeFiles/dprot_pract2.dir/depend:
	cd /home/manel/CLionProjects/dprot/dprot_pract2_4/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/manel/CLionProjects/dprot/dprot_pract2_4 /home/manel/CLionProjects/dprot/dprot_pract2_4 /home/manel/CLionProjects/dprot/dprot_pract2_4/cmake-build-debug /home/manel/CLionProjects/dprot/dprot_pract2_4/cmake-build-debug /home/manel/CLionProjects/dprot/dprot_pract2_4/cmake-build-debug/CMakeFiles/dprot_pract2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/dprot_pract2.dir/depend

