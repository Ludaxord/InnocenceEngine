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
include Engine/ThirdParty/JSONWrapper/CMakeFiles/JSONWrapper.dir/depend.make

# Include the progress variables for this target.
include Engine/ThirdParty/JSONWrapper/CMakeFiles/JSONWrapper.dir/progress.make

# Include the compile flags for this target's objects.
include Engine/ThirdParty/JSONWrapper/CMakeFiles/JSONWrapper.dir/flags.make

Engine/ThirdParty/JSONWrapper/CMakeFiles/JSONWrapper.dir/JSONWrapper.cpp.o: Engine/ThirdParty/JSONWrapper/CMakeFiles/JSONWrapper.dir/flags.make
Engine/ThirdParty/JSONWrapper/CMakeFiles/JSONWrapper.dir/JSONWrapper.cpp.o: ../Engine/ThirdParty/JSONWrapper/JSONWrapper.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Engine/ThirdParty/JSONWrapper/CMakeFiles/JSONWrapper.dir/JSONWrapper.cpp.o"
	cd /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/Engine/ThirdParty/JSONWrapper && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/JSONWrapper.dir/JSONWrapper.cpp.o -c /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/Engine/ThirdParty/JSONWrapper/JSONWrapper.cpp

Engine/ThirdParty/JSONWrapper/CMakeFiles/JSONWrapper.dir/JSONWrapper.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/JSONWrapper.dir/JSONWrapper.cpp.i"
	cd /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/Engine/ThirdParty/JSONWrapper && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/Engine/ThirdParty/JSONWrapper/JSONWrapper.cpp > CMakeFiles/JSONWrapper.dir/JSONWrapper.cpp.i

Engine/ThirdParty/JSONWrapper/CMakeFiles/JSONWrapper.dir/JSONWrapper.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/JSONWrapper.dir/JSONWrapper.cpp.s"
	cd /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/Engine/ThirdParty/JSONWrapper && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/Engine/ThirdParty/JSONWrapper/JSONWrapper.cpp -o CMakeFiles/JSONWrapper.dir/JSONWrapper.cpp.s

# Object files for target JSONWrapper
JSONWrapper_OBJECTS = \
"CMakeFiles/JSONWrapper.dir/JSONWrapper.cpp.o"

# External object files for target JSONWrapper
JSONWrapper_EXTERNAL_OBJECTS =

LibArchive/libJSONWrapper.a: Engine/ThirdParty/JSONWrapper/CMakeFiles/JSONWrapper.dir/JSONWrapper.cpp.o
LibArchive/libJSONWrapper.a: Engine/ThirdParty/JSONWrapper/CMakeFiles/JSONWrapper.dir/build.make
LibArchive/libJSONWrapper.a: Engine/ThirdParty/JSONWrapper/CMakeFiles/JSONWrapper.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library ../../../LibArchive/libJSONWrapper.a"
	cd /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/Engine/ThirdParty/JSONWrapper && $(CMAKE_COMMAND) -P CMakeFiles/JSONWrapper.dir/cmake_clean_target.cmake
	cd /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/Engine/ThirdParty/JSONWrapper && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/JSONWrapper.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Engine/ThirdParty/JSONWrapper/CMakeFiles/JSONWrapper.dir/build: LibArchive/libJSONWrapper.a

.PHONY : Engine/ThirdParty/JSONWrapper/CMakeFiles/JSONWrapper.dir/build

Engine/ThirdParty/JSONWrapper/CMakeFiles/JSONWrapper.dir/clean:
	cd /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/Engine/ThirdParty/JSONWrapper && $(CMAKE_COMMAND) -P CMakeFiles/JSONWrapper.dir/cmake_clean.cmake
.PHONY : Engine/ThirdParty/JSONWrapper/CMakeFiles/JSONWrapper.dir/clean

Engine/ThirdParty/JSONWrapper/CMakeFiles/JSONWrapper.dir/depend:
	cd /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/Engine/ThirdParty/JSONWrapper /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/Engine/ThirdParty/JSONWrapper /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/Engine/ThirdParty/JSONWrapper/CMakeFiles/JSONWrapper.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Engine/ThirdParty/JSONWrapper/CMakeFiles/JSONWrapper.dir/depend
