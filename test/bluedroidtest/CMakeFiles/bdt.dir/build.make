# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/nikhil/Documents/bluedroid.for.linux

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nikhil/Documents/bluedroid.for.linux

# Include any dependencies generated for this target.
include test/bluedroidtest/CMakeFiles/bdt.dir/depend.make

# Include the progress variables for this target.
include test/bluedroidtest/CMakeFiles/bdt.dir/progress.make

# Include the compile flags for this target's objects.
include test/bluedroidtest/CMakeFiles/bdt.dir/flags.make

test/bluedroidtest/CMakeFiles/bdt.dir/bluedroidtest.c.o: test/bluedroidtest/CMakeFiles/bdt.dir/flags.make
test/bluedroidtest/CMakeFiles/bdt.dir/bluedroidtest.c.o: test/bluedroidtest/bluedroidtest.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nikhil/Documents/bluedroid.for.linux/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object test/bluedroidtest/CMakeFiles/bdt.dir/bluedroidtest.c.o"
	cd /home/nikhil/Documents/bluedroid.for.linux/test/bluedroidtest && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/bdt.dir/bluedroidtest.c.o   -c /home/nikhil/Documents/bluedroid.for.linux/test/bluedroidtest/bluedroidtest.c

test/bluedroidtest/CMakeFiles/bdt.dir/bluedroidtest.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bdt.dir/bluedroidtest.c.i"
	cd /home/nikhil/Documents/bluedroid.for.linux/test/bluedroidtest && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/nikhil/Documents/bluedroid.for.linux/test/bluedroidtest/bluedroidtest.c > CMakeFiles/bdt.dir/bluedroidtest.c.i

test/bluedroidtest/CMakeFiles/bdt.dir/bluedroidtest.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bdt.dir/bluedroidtest.c.s"
	cd /home/nikhil/Documents/bluedroid.for.linux/test/bluedroidtest && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/nikhil/Documents/bluedroid.for.linux/test/bluedroidtest/bluedroidtest.c -o CMakeFiles/bdt.dir/bluedroidtest.c.s

# Object files for target bdt
bdt_OBJECTS = \
"CMakeFiles/bdt.dir/bluedroidtest.c.o"

# External object files for target bdt
bdt_EXTERNAL_OBJECTS =

test/bluedroidtest/bdt: test/bluedroidtest/CMakeFiles/bdt.dir/bluedroidtest.c.o
test/bluedroidtest/bdt: test/bluedroidtest/CMakeFiles/bdt.dir/build.make
test/bluedroidtest/bdt: main/bluetooth.default.so
test/bluedroidtest/bdt: hci/libbt-hci.so
test/bluedroidtest/bdt: gki/libbt-brcm_gki.a
test/bluedroidtest/bdt: stack/libbt-brcm_stack.a
test/bluedroidtest/bdt: bta/libbt-brcm_bta.a
test/bluedroidtest/bdt: utils/libbt-utils.a
test/bluedroidtest/bdt: stack/libbt-brcm_stack.a
test/bluedroidtest/bdt: test/bluedroidtest/CMakeFiles/bdt.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/nikhil/Documents/bluedroid.for.linux/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bdt"
	cd /home/nikhil/Documents/bluedroid.for.linux/test/bluedroidtest && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bdt.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/bluedroidtest/CMakeFiles/bdt.dir/build: test/bluedroidtest/bdt

.PHONY : test/bluedroidtest/CMakeFiles/bdt.dir/build

test/bluedroidtest/CMakeFiles/bdt.dir/clean:
	cd /home/nikhil/Documents/bluedroid.for.linux/test/bluedroidtest && $(CMAKE_COMMAND) -P CMakeFiles/bdt.dir/cmake_clean.cmake
.PHONY : test/bluedroidtest/CMakeFiles/bdt.dir/clean

test/bluedroidtest/CMakeFiles/bdt.dir/depend:
	cd /home/nikhil/Documents/bluedroid.for.linux && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nikhil/Documents/bluedroid.for.linux /home/nikhil/Documents/bluedroid.for.linux/test/bluedroidtest /home/nikhil/Documents/bluedroid.for.linux /home/nikhil/Documents/bluedroid.for.linux/test/bluedroidtest /home/nikhil/Documents/bluedroid.for.linux/test/bluedroidtest/CMakeFiles/bdt.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/bluedroidtest/CMakeFiles/bdt.dir/depend

