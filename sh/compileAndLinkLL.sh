#!/usr/bin/env bash

# Ensure we got correct args count.
if [[ ! "$#" -gt 4 ]]; then
  echo "Incorrect argument count."
  echo "Usage: ${0} <name> <version> <destination dir> <include dir> <... .c to compile ...>"
  exit 1
fi

# Set up name variables.
VER="${2}"
PRGM="${1}"

# Set up paths.
DEST_DIR=`realpath "${3}"`
INC_DIR=`realpath "${4}"`
PRGM_DIR="${DEST_DIR}/${PRGM}"
LL_DIR="${PRGM_DIR}/${VER}LL"

# Build dest dir.
if [[ ! -d "${DEST_DIR}" ]]; then
  echo "Making destination directory: ${DEST_DIR}"
  mkdir -p "${DEST_DIR}"
fi

# Build program dir.
if [[ ! -d "${PRGM_DIR}" ]]; then
  echo "Making program directory: ${PRGM_DIR}"
  mkdir -p "${PRGM_DIR}"
fi

# Remove old build version.
if [[ -d "${LL_DIR}" ]]; then
  echo "Removing old version directory: ${LL_DIR}"
  rm -rf "${LL_DIR}"
fi

# Build version directory.
if [[ ! -d "${LL_DIR}" ]]; then
  echo "Making version directory: ${LL_DIR}"
  mkdir -p "${LL_DIR}"
fi

# Make absolute paths to files.
declare -a C_FILES
for f in "${@:5}"; do
  C_FILES+=(`realpath "${f}"`)
done
echo "Got files: ${C_FILES[@]}"

# Change to destination directory.
echo "Compiling to: ${LL_DIR}"
cd "${LL_DIR}"

# Create destination paths.
declare -a LL_FILES
for f in "${C_FILES[@]}"; do
  FILE_NAME=`basename ${f}`
  LL_FILES+=("${LL_DIR}/${FILE_NAME/.c/.ll}")
done
unset FILE_NAME
echo "Creating files: ${LL_FILES[@]}"

# Build command.
BUILD_CMD="clang -S -emit-llvm ${C_FILES[@]} -I${INC_DIR}"
echo "${BUILD_CMD}"
eval "${BUILD_CMD}"

# Link command.
echo "${LL_FILES[@]}"
LINK_CMD="llvm-link -o ${PRGM_DIR}/${VER}.bc ${LL_FILES[@]}"
echo "${LINK_CMD}"
eval "${LINK_CMD}"
