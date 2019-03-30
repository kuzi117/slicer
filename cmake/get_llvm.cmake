# This file was written by Braedy Kuzma for the CMPUT 415 class, 2018 at the University of Alberta.
# This file was modified as part of slicer by Braedy Kuzma, 2019.

# Need LLVM so we can link against it. Either we automatically find this in the "default" place or
# it's found because we set up $LLVM_DIR. There should be no additions necessary here.
find_package(LLVM REQUIRED CONFIG)

# Status messages about LLVM found.
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Ensure we found our own specified version at 8.0.0.
if(NOT ("${LLVM_VERSION_MAJOR}" EQUAL 8 AND
  "${LLVM_VERSION_MINOR}" EQUAL 0 AND
  "${LLVM_VERSION_PATCH}" EQUAL 0))
  message(FATAL_ERROR "LLVM version incompatible.")
endif()

# Add llvm specific pieces to our build.
include_directories("${LLVM_INCLUDE_DIRS}")
add_definitions("${LLVM_DEFINITIONS}")
