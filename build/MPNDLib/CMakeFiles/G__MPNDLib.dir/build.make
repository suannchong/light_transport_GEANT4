# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/suannchong/Desktop/Geant4/mpnd_suann

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/suannchong/Desktop/Geant4/mpnd_suann/build

# Utility rule file for G__MPNDLib.

# Include the progress variables for this target.
include MPNDLib/CMakeFiles/G__MPNDLib.dir/progress.make

MPNDLib/CMakeFiles/G__MPNDLib: MPNDLib/G__MPNDLib.cxx
MPNDLib/CMakeFiles/G__MPNDLib: MPNDLib/libMPNDLib_rdict.pcm
MPNDLib/CMakeFiles/G__MPNDLib: MPNDLib/libMPNDLib.rootmap


MPNDLib/G__MPNDLib.cxx: MPNDLib/LinkDef.h
MPNDLib/G__MPNDLib.cxx: ../mpndlib/include/mpndglobals.hpp
MPNDLib/G__MPNDLib.cxx: ../mpndlib/include/simhit.hpp
MPNDLib/G__MPNDLib.cxx: ../mpndlib/include/mpndglobals.hpp
MPNDLib/G__MPNDLib.cxx: ../mpndlib/include/simhit.hpp
MPNDLib/G__MPNDLib.cxx: MPNDLib/LinkDef.h
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/suannchong/Desktop/Geant4/mpnd_suann/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating G__MPNDLib.cxx, libMPNDLib_rdict.pcm, libMPNDLib.rootmap"
	cd /Users/suannchong/Desktop/Geant4/mpnd_suann/build/MPNDLib && /Applications/CMake.app/Contents/bin/cmake -E env LD_LIBRARY_PATH=/Applications/root_v6.16.00/lib:/Applications/root_v6.16.00/lib /Applications/root_v6.16.00/bin/rootcling -v2 -f G__MPNDLib.cxx -s /Users/suannchong/Desktop/Geant4/mpnd_suann/build/MPNDLib/libMPNDLib.dylib -rml libMPNDLib.dylib -rmf /Users/suannchong/Desktop/Geant4/mpnd_suann/build/MPNDLib/libMPNDLib.rootmap -I/Applications/root_v6.16.00/include -I/Users/suannchong/Desktop/Geant4/mpnd_suann/mpndlib/include /Users/suannchong/Desktop/Geant4/mpnd_suann/mpndlib/include/mpndglobals.hpp /Users/suannchong/Desktop/Geant4/mpnd_suann/mpndlib/include/simhit.hpp /Users/suannchong/Desktop/Geant4/mpnd_suann/build/MPNDLib/LinkDef.h

MPNDLib/libMPNDLib_rdict.pcm: MPNDLib/G__MPNDLib.cxx
	@$(CMAKE_COMMAND) -E touch_nocreate MPNDLib/libMPNDLib_rdict.pcm

MPNDLib/libMPNDLib.rootmap: MPNDLib/G__MPNDLib.cxx
	@$(CMAKE_COMMAND) -E touch_nocreate MPNDLib/libMPNDLib.rootmap

G__MPNDLib: MPNDLib/CMakeFiles/G__MPNDLib
G__MPNDLib: MPNDLib/G__MPNDLib.cxx
G__MPNDLib: MPNDLib/libMPNDLib_rdict.pcm
G__MPNDLib: MPNDLib/libMPNDLib.rootmap
G__MPNDLib: MPNDLib/CMakeFiles/G__MPNDLib.dir/build.make

.PHONY : G__MPNDLib

# Rule to build all files generated by this target.
MPNDLib/CMakeFiles/G__MPNDLib.dir/build: G__MPNDLib

.PHONY : MPNDLib/CMakeFiles/G__MPNDLib.dir/build

MPNDLib/CMakeFiles/G__MPNDLib.dir/clean:
	cd /Users/suannchong/Desktop/Geant4/mpnd_suann/build/MPNDLib && $(CMAKE_COMMAND) -P CMakeFiles/G__MPNDLib.dir/cmake_clean.cmake
.PHONY : MPNDLib/CMakeFiles/G__MPNDLib.dir/clean

MPNDLib/CMakeFiles/G__MPNDLib.dir/depend:
	cd /Users/suannchong/Desktop/Geant4/mpnd_suann/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/suannchong/Desktop/Geant4/mpnd_suann /Users/suannchong/Desktop/Geant4/mpnd_suann/mpndlib /Users/suannchong/Desktop/Geant4/mpnd_suann/build /Users/suannchong/Desktop/Geant4/mpnd_suann/build/MPNDLib /Users/suannchong/Desktop/Geant4/mpnd_suann/build/MPNDLib/CMakeFiles/G__MPNDLib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : MPNDLib/CMakeFiles/G__MPNDLib.dir/depend

