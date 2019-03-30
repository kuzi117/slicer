# Check if our compiler is Clang, we need it to run all of our tools and
# differencing.
if (NOT "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
  message(STATUS "Using compiler: ${CMAKE_C_COMPILER_ID}")
  message(FATAL_ERROR "Using slice requires installing Clang/LLVM 8.0.0")
endif()

set(SLICE_NAME "slice")

function(add_slice_test_suite SUITE_NAME)
  set(PRGM_C_FILES "${ARGN}")
  _compLL("${SUITE_NAME}" "${TEST_C_FILES}")
endfunction(add_slice_test_suite)

# A function that compiles .c files to .ll files in the build directory.
# Args:
#   SUITE_NAME: The name of the suite to compile.
#
function(_compLL SUITE_NAME)
  # Paths for dirs.
  set(SLICE_DEST_DIR "${CMAKE_BINARY_DIR}/${SLICE_NAME}")
  set(SUITE_DEST_DIR "${SLICE_DEST_DIR}/${SUITE_NAME}")
  set(LL_DEST_DIR "${SUITE_DEST_DIR}/ll")

  # Create base directory if necessary.
  if(NOT EXISTS "${SLICE_DEST_DIR}")
    message(STATUS "Creating ${SLICE_NAME} dir: ${SLICE_DEST_DIR}")
    file(MAKE_DIRECTORY "${SLICE_DEST_DIR}")
  endif()

  # COMPLETE REFRESH of suite files.
  # TODO: This can be smarter, we only need to rebuild some files.
  if(EXISTS ${SUITE_DEST_DIR})
    message(STATUS "Removing old ${SUITE_NAME} suite dir.")
    file(REMOVE "${SLICE_DEST_DIR}")
  endif()

  # Add new directories.
  message(STATUS "Creating ${SUITE_NAME} suite dir: ${SUITE_DEST_DIR}")
  file(MAKE_DIRECTORY "${SUITE_DEST_DIR}")
  message(STATUS "Creating ${LL_DEST_DIR} suite ll dir: ${LL_DEST_DIR}")
  file(MAKE_DIRECTORY "${LL_DEST_DIR}")

  # Get inputs/outpts.
  set(PRGM_C_FILES "${ARGN}")
  set(TEST_LL_FILES )

  # Setup files.
  message(STATUS "Adding _compLL_${SUITE_NAME} target.")

  add_custom_command(
    "${CMAKE_C_COMPILER} -S -emit-llvm
  )


endfunction(_compLL)

