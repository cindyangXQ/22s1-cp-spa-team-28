# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

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

# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/benedict/Workspace/22s1-cp-spa-team-28/Team28/Code28

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/benedict/Workspace/22s1-cp-spa-team-28/Team28/Code28/build2

# Include any dependencies generated for this target.
include src/autotester/CMakeFiles/autotester.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/autotester/CMakeFiles/autotester.dir/compiler_depend.make

# Include the progress variables for this target.
include src/autotester/CMakeFiles/autotester.dir/progress.make

# Include the compile flags for this target's objects.
include src/autotester/CMakeFiles/autotester.dir/flags.make

src/autotester/CMakeFiles/autotester.dir/src/TestWrapper.cpp.o: src/autotester/CMakeFiles/autotester.dir/flags.make
src/autotester/CMakeFiles/autotester.dir/src/TestWrapper.cpp.o: /Users/benedict/Workspace/22s1-cp-spa-team-28/Team28/Code28/src/autotester/src/TestWrapper.cpp
src/autotester/CMakeFiles/autotester.dir/src/TestWrapper.cpp.o: src/autotester/CMakeFiles/autotester.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/benedict/Workspace/22s1-cp-spa-team-28/Team28/Code28/build2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/autotester/CMakeFiles/autotester.dir/src/TestWrapper.cpp.o"
	cd /Users/benedict/Workspace/22s1-cp-spa-team-28/Team28/Code28/build2/src/autotester && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/autotester/CMakeFiles/autotester.dir/src/TestWrapper.cpp.o -MF CMakeFiles/autotester.dir/src/TestWrapper.cpp.o.d -o CMakeFiles/autotester.dir/src/TestWrapper.cpp.o -c /Users/benedict/Workspace/22s1-cp-spa-team-28/Team28/Code28/src/autotester/src/TestWrapper.cpp

src/autotester/CMakeFiles/autotester.dir/src/TestWrapper.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/autotester.dir/src/TestWrapper.cpp.i"
	cd /Users/benedict/Workspace/22s1-cp-spa-team-28/Team28/Code28/build2/src/autotester && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/benedict/Workspace/22s1-cp-spa-team-28/Team28/Code28/src/autotester/src/TestWrapper.cpp > CMakeFiles/autotester.dir/src/TestWrapper.cpp.i

src/autotester/CMakeFiles/autotester.dir/src/TestWrapper.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/autotester.dir/src/TestWrapper.cpp.s"
	cd /Users/benedict/Workspace/22s1-cp-spa-team-28/Team28/Code28/build2/src/autotester && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/benedict/Workspace/22s1-cp-spa-team-28/Team28/Code28/src/autotester/src/TestWrapper.cpp -o CMakeFiles/autotester.dir/src/TestWrapper.cpp.s

# Object files for target autotester
autotester_OBJECTS = \
"CMakeFiles/autotester.dir/src/TestWrapper.cpp.o"

# External object files for target autotester
autotester_EXTERNAL_OBJECTS =

src/autotester/autotester: src/autotester/CMakeFiles/autotester.dir/src/TestWrapper.cpp.o
src/autotester/autotester: src/autotester/CMakeFiles/autotester.dir/build.make
src/autotester/autotester: src/spa/libspa.a
src/autotester/autotester: /Users/benedict/Workspace/22s1-cp-spa-team-28/Team28/Code28/lib/autotester/x86_64/Darwin/Release/libautotester.a
src/autotester/autotester: src/spa/src/commons/libcommons.a
src/autotester/autotester: src/spa/src/pkb/libpkb.a
src/autotester/autotester: src/spa/src/qps/libqps.a
src/autotester/autotester: src/spa/src/sp/libsp.a
src/autotester/autotester: src/autotester/CMakeFiles/autotester.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/benedict/Workspace/22s1-cp-spa-team-28/Team28/Code28/build2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable autotester"
	cd /Users/benedict/Workspace/22s1-cp-spa-team-28/Team28/Code28/build2/src/autotester && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/autotester.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/autotester/CMakeFiles/autotester.dir/build: src/autotester/autotester
.PHONY : src/autotester/CMakeFiles/autotester.dir/build

src/autotester/CMakeFiles/autotester.dir/clean:
	cd /Users/benedict/Workspace/22s1-cp-spa-team-28/Team28/Code28/build2/src/autotester && $(CMAKE_COMMAND) -P CMakeFiles/autotester.dir/cmake_clean.cmake
.PHONY : src/autotester/CMakeFiles/autotester.dir/clean

src/autotester/CMakeFiles/autotester.dir/depend:
	cd /Users/benedict/Workspace/22s1-cp-spa-team-28/Team28/Code28/build2 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/benedict/Workspace/22s1-cp-spa-team-28/Team28/Code28 /Users/benedict/Workspace/22s1-cp-spa-team-28/Team28/Code28/src/autotester /Users/benedict/Workspace/22s1-cp-spa-team-28/Team28/Code28/build2 /Users/benedict/Workspace/22s1-cp-spa-team-28/Team28/Code28/build2/src/autotester /Users/benedict/Workspace/22s1-cp-spa-team-28/Team28/Code28/build2/src/autotester/CMakeFiles/autotester.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/autotester/CMakeFiles/autotester.dir/depend

