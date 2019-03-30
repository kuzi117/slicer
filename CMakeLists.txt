cmake_minimum_required(VERSION 3.12)
project(slicer)

# Set our standard.
set(CMAKE_CXX_STANDARD 11)

# Find llvm.
include("${CMAKE_SOURCE_DIR}/cmake/get_llvm.cmake")

# Add our own include directories.
include_directories("${CMAKE_SOURCE_DIR}/include")

# Start the build process.
add_subdirectory("${CMAKE_SOURCE_DIR}/src")
add_subdirectory("${CMAKE_SOURCE_DIR}/lib")
