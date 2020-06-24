# CMakeLists for MPNDG4
cmake_minimum_required(VERSION 3.5)

# Set up project
project(mpndg_suann)
set(MPNDG4_VERSION_MAJOR 0)
set(MPNDG4_VERSION_MINOR 6)
set(MPNDG4_VERSION_PATCH 0)
set(MPNDG4_VERSION ${MPNDG4_VERSION_MAJOR}.${MPNDG4_VERSION_MINOR}.${MPNDG4_VERSION_PATCH})
message(STATUS "Building MPNDG4 v${MPNDG4_VERSION}")

# C++ options
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Bring in MPNDLib via its CMakeLists
add_subdirectory(${PROJECT_SOURCE_DIR}/mpndlib MPNDLib)

# Geant4 setup
option(MPNDG4_VIS "Build with Geant4 UI and visualization" ON)
option(MPNDG4_MT "Build with multithreading. G4MULTITHREADED must also be true." ON)
if (MPNDG4_VIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()
include(${Geant4_USE_FILE})
message(STATUS "Geant4_USE_FILE = ${Geant4_USE_FILE}")

# Pull in ROOT setup info
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})
message(STATUS "ROOT_USE_FILE = ${ROOT_USE_FILE}")

# Set build options, add includes, and bring in other source directories
include(${PROJECT_SOURCE_DIR}/cmake/BuildOptions.cmake)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/mpndlib/include)
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# MPNDG4 setup
add_executable(sim ${SOURCES})
target_link_libraries(sim ${Geant4_LIBRARIES})
target_link_libraries(sim MPNDLib)
if(${MPNDG4_MT})
  target_compile_definitions(sim PUBLIC "-DMPNDG4MT=1")
  message(STATUS "Building with multithreading enabled.")
else()
  message(STATUS "Building in sequential mode.")
endif()
if(${MPNDG4_VIS})
  target_compile_definitions(sim PUBLIC "-DMPNDG4VIS=1")
  message(STATUS "Building with visualization enabled.")
else()
  message(STATUS "Building without visualization.")
endif()

# Copy init macro files and examples to build directory
file(GLOB INIT_MAC ${PROJECT_SOURCE_DIR}/scripts/init_mac/*.mac)
file(COPY ${INIT_MAC} DESTINATION ${PROJECT_BINARY_DIR}/init_mac)
file(GLOB EX_MAC ${PROJECT_SOURCE_DIR}/examples/*.mac)
file(COPY ${EX_MAC} DESTINATION ${PROJECT_BINARY_DIR}/examples)
