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
include Engine/ThirdParty/STBWrapper/CMakeFiles/STBWrapper.dir/depend.make

# Include the progress variables for this target.
include Engine/ThirdParty/STBWrapper/CMakeFiles/STBWrapper.dir/progress.make

# Include the compile flags for this target's objects.
include Engine/ThirdParty/STBWrapper/CMakeFiles/STBWrapper.dir/flags.make

Engine/ThirdParty/STBWrapper/CMakeFiles/STBWrapper.dir/STBWrapper.cpp.o: Engine/ThirdParty/STBWrapper/CMakeFiles/STBWrapper.dir/flags.make
Engine/ThirdParty/STBWrapper/CMakeFiles/STBWrapper.dir/STBWrapper.cpp.o: ../Engine/ThirdParty/STBWrapper/STBWrapper.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Engine/ThirdParty/STBWrapper/CMakeFiles/STBWrapper.dir/STBWrapper.cpp.o"
	cd /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/Engine/ThirdParty/STBWrapper && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/STBWrapper.dir/STBWrapper.cpp.o -c /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/Engine/ThirdParty/STBWrapper/STBWrapper.cpp

Engine/ThirdParty/STBWrapper/CMakeFiles/STBWrapper.dir/STBWrapper.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/STBWrapper.dir/STBWrapper.cpp.i"
	cd /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/Engine/ThirdParty/STBWrapper && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/Engine/ThirdParty/STBWrapper/STBWrapper.cpp > CMakeFiles/STBWrapper.dir/STBWrapper.cpp.i

Engine/ThirdParty/STBWrapper/CMakeFiles/STBWrapper.dir/STBWrapper.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/STBWrapper.dir/STBWrapper.cpp.s"
	cd /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/Engine/ThirdParty/STBWrapper && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/Engine/ThirdParty/STBWrapper/STBWrapper.cpp -o CMakeFiles/STBWrapper.dir/STBWrapper.cpp.s

Engine/ThirdParty/STBWrapper/CMakeFiles/STBWrapper.dir/stb_image.cpp.o: Engine/ThirdParty/STBWrapper/CMakeFiles/STBWrapper.dir/flags.make
Engine/ThirdParty/STBWrapper/CMakeFiles/STBWrapper.dir/stb_image.cpp.o: ../Engine/ThirdParty/STBWrapper/stb_image.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object Engine/ThirdParty/STBWrapper/CMakeFiles/STBWrapper.dir/stb_image.cpp.o"
	cd /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/Engine/ThirdParty/STBWrapper && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/STBWrapper.dir/stb_image.cpp.o -c /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/Engine/ThirdParty/STBWrapper/stb_image.cpp

Engine/ThirdParty/STBWrapper/CMakeFiles/STBWrapper.dir/stb_image.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/STBWrapper.dir/stb_image.cpp.i"
	cd /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/Engine/ThirdParty/STBWrapper && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/Engine/ThirdParty/STBWrapper/stb_image.cpp > CMakeFiles/STBWrapper.dir/stb_image.cpp.i

Engine/ThirdParty/STBWrapper/CMakeFiles/STBWrapper.dir/stb_image.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/STBWrapper.dir/stb_image.cpp.s"
	cd /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/Engine/ThirdParty/STBWrapper && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/Engine/ThirdParty/STBWrapper/stb_image.cpp -o CMakeFiles/STBWrapper.dir/stb_image.cpp.s

Engine/ThirdParty/STBWrapper/CMakeFiles/STBWrapper.dir/stb_image_write.cpp.o: Engine/ThirdParty/STBWrapper/CMakeFiles/STBWrapper.dir/flags.make
Engine/ThirdParty/STBWrapper/CMakeFiles/STBWrapper.dir/stb_image_write.cpp.o: ../Engine/ThirdParty/STBWrapper/stb_image_write.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object Engine/ThirdParty/STBWrapper/CMakeFiles/STBWrapper.dir/stb_image_write.cpp.o"
	cd /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/Engine/ThirdParty/STBWrapper && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/STBWrapper.dir/stb_image_write.cpp.o -c /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/Engine/ThirdParty/STBWrapper/stb_image_write.cpp

Engine/ThirdParty/STBWrapper/CMakeFiles/STBWrapper.dir/stb_image_write.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/STBWrapper.dir/stb_image_write.cpp.i"
	cd /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/Engine/ThirdParty/STBWrapper && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/Engine/ThirdParty/STBWrapper/stb_image_write.cpp > CMakeFiles/STBWrapper.dir/stb_image_write.cpp.i

Engine/ThirdParty/STBWrapper/CMakeFiles/STBWrapper.dir/stb_image_write.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/STBWrapper.dir/stb_image_write.cpp.s"
	cd /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/Engine/ThirdParty/STBWrapper && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/Engine/ThirdParty/STBWrapper/stb_image_write.cpp -o CMakeFiles/STBWrapper.dir/stb_image_write.cpp.s

# Object files for target STBWrapper
STBWrapper_OBJECTS = \
"CMakeFiles/STBWrapper.dir/STBWrapper.cpp.o" \
"CMakeFiles/STBWrapper.dir/stb_image.cpp.o" \
"CMakeFiles/STBWrapper.dir/stb_image_write.cpp.o"

# External object files for target STBWrapper
STBWrapper_EXTERNAL_OBJECTS =

LibArchive/libSTBWrapper.a: Engine/ThirdParty/STBWrapper/CMakeFiles/STBWrapper.dir/STBWrapper.cpp.o
LibArchive/libSTBWrapper.a: Engine/ThirdParty/STBWrapper/CMakeFiles/STBWrapper.dir/stb_image.cpp.o
LibArchive/libSTBWrapper.a: Engine/ThirdParty/STBWrapper/CMakeFiles/STBWrapper.dir/stb_image_write.cpp.o
LibArchive/libSTBWrapper.a: Engine/ThirdParty/STBWrapper/CMakeFiles/STBWrapper.dir/build.make
LibArchive/libSTBWrapper.a: Engine/ThirdParty/STBWrapper/CMakeFiles/STBWrapper.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX static library ../../../LibArchive/libSTBWrapper.a"
	cd /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/Engine/ThirdParty/STBWrapper && $(CMAKE_COMMAND) -P CMakeFiles/STBWrapper.dir/cmake_clean_target.cmake
	cd /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/Engine/ThirdParty/STBWrapper && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/STBWrapper.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Engine/ThirdParty/STBWrapper/CMakeFiles/STBWrapper.dir/build: LibArchive/libSTBWrapper.a

.PHONY : Engine/ThirdParty/STBWrapper/CMakeFiles/STBWrapper.dir/build

Engine/ThirdParty/STBWrapper/CMakeFiles/STBWrapper.dir/clean:
	cd /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/Engine/ThirdParty/STBWrapper && $(CMAKE_COMMAND) -P CMakeFiles/STBWrapper.dir/cmake_clean.cmake
.PHONY : Engine/ThirdParty/STBWrapper/CMakeFiles/STBWrapper.dir/clean

Engine/ThirdParty/STBWrapper/CMakeFiles/STBWrapper.dir/depend:
	cd /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/Engine/ThirdParty/STBWrapper /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/Engine/ThirdParty/STBWrapper /Volumes/Extreme/Projects/Personal/GameEngineResources/innocence_engine/InnocenceEngine/Source/cmake-build-debug/Engine/ThirdParty/STBWrapper/CMakeFiles/STBWrapper.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Engine/ThirdParty/STBWrapper/CMakeFiles/STBWrapper.dir/depend

