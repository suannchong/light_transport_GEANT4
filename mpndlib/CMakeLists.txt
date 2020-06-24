# CMakeLists for MPNDLib
cmake_minimum_required(VERSION 3.5)
# Set up project
project(mpndlib)
set(PROJ_TARGET "MPNDLib")
message(STATUS "Building ${PROJ_TARGET}")

# C++ options
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate linkdefs - only 1 file, LinkDef.h, which will be in the build
# directory
file(GLOB MPNDLIB_HEADERS ${PROJECT_SOURCE_DIR}/include/*.hpp)
set(LINKDEF ${PROJECT_BINARY_DIR}/LinkDef.h)
set(LD_HEADER
"#ifdef __CINT__

#pragma link off all globals\;
#pragma link off all classes\;
#pragma link off all functions\;
#pragma link C++ nestedclasses\;\n")
file(WRITE ${LINKDEF} ${LD_HEADER})
foreach(HEADER ${MPNDLIB_HEADERS})
  file(APPEND ${LINKDEF} "#pragma link C++ defined_in ${HEADER};\n")
endforeach()
file(APPEND ${LINKDEF} "\n#endif\n")

# Pull in ROOT setup info
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})
message(STATUS "ROOT_USE_FILE = ${ROOT_USE_FILE}")

# Set build options, add includes, and bring in other source directories
# must include() before root_generate_dictionary!
include(${PROJECT_SOURCE_DIR}/../cmake/BuildOptions.cmake)
include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB MPNDLIB_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Generate ROOT dictionaries. The .cxx, .rootmap, and .pcm  will go into
# the build directory
message(STATUS "Running ROOT_GENERATE_DICTIONARY on ${PROJ_TARGET}")
root_generate_dictionary(G__MPNDLib ${MPNDLIB_HEADERS} LINKDEF ${LINKDEF})

# Make the lib
add_library(${PROJ_TARGET} SHARED ${MPNDLIB_SOURCES} G__MPNDLib.cxx)
target_link_libraries(${PROJ_TARGET} ${ROOT_LIBRARIES})
