# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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
CMAKE_COMMAND = "/Users/konraduciechowski/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/202.7319.72/CLion.app/Contents/bin/cmake/mac/bin/cmake"

# The command to remove a file.
RM = "/Users/konraduciechowski/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/202.7319.72/CLion.app/Contents/bin/cmake/mac/bin/cmake" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug

# Include any dependencies generated for this target.
include Test/CMakeFiles/InnoTest.dir/depend.make

# Include the progress variables for this target.
include Test/CMakeFiles/InnoTest.dir/progress.make

# Include the compile flags for this target's objects.
include Test/CMakeFiles/InnoTest.dir/flags.make

Test/CMakeFiles/InnoTest.dir/InnoTest.cpp.o: Test/CMakeFiles/InnoTest.dir/flags.make
Test/CMakeFiles/InnoTest.dir/InnoTest.cpp.o: ../Test/InnoTest.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Test/CMakeFiles/InnoTest.dir/InnoTest.cpp.o"
	cd /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/Test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/InnoTest.dir/InnoTest.cpp.o -c /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/Test/InnoTest.cpp

Test/CMakeFiles/InnoTest.dir/InnoTest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/InnoTest.dir/InnoTest.cpp.i"
	cd /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/Test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/Test/InnoTest.cpp > CMakeFiles/InnoTest.dir/InnoTest.cpp.i

Test/CMakeFiles/InnoTest.dir/InnoTest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/InnoTest.dir/InnoTest.cpp.s"
	cd /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/Test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/Test/InnoTest.cpp -o CMakeFiles/InnoTest.dir/InnoTest.cpp.s

# Object files for target InnoTest
InnoTest_OBJECTS = \
"CMakeFiles/InnoTest.dir/InnoTest.cpp.o"

# External object files for target InnoTest
InnoTest_EXTERNAL_OBJECTS =

/Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Bin/InnoTest: Test/CMakeFiles/InnoTest.dir/InnoTest.cpp.o
/Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Bin/InnoTest: Test/CMakeFiles/InnoTest.dir/build.make
/Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Bin/InnoTest: LibArchive/libInnoCore.a
/Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Bin/InnoTest: Test/CMakeFiles/InnoTest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Bin/InnoTest"
	cd /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/Test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/InnoTest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Test/CMakeFiles/InnoTest.dir/build: /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Bin/InnoTest

.PHONY : Test/CMakeFiles/InnoTest.dir/build

Test/CMakeFiles/InnoTest.dir/clean:
	cd /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/Test && $(CMAKE_COMMAND) -P CMakeFiles/InnoTest.dir/cmake_clean.cmake
.PHONY : Test/CMakeFiles/InnoTest.dir/clean

Test/CMakeFiles/InnoTest.dir/depend:
	cd /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/Test /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/Test /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/Test/CMakeFiles/InnoTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Test/CMakeFiles/InnoTest.dir/depend
