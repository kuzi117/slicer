#!/usr/bin/env bash

# If the since variable is not set we have nothing to compare to.
if [ -z "${since}" ]; then
  echo "The \$since variable was not set!"
  echo "We can't make tests if we have nothing to compare to!"
  exit 1
fi

# Hold onto base dir and get base commit so we always know where to go.
projName=`basename ${PWD}`
base=`pwd`
curSHA=`git log --format="%h" | head -n 1`
oldSHA="${since}"

# TODO Can't depend on the system opt version, need to check if opt version
opt="${LLVM_BIN}/opt"

# Get the slicer.
git clone https://github.com/kuzi117/slicer.git
cd slicer
slicer=`pwd`

# Make the slicer tools.
cmake .
make -j4

# Set up executables.
differ="${slicer}/src/differ/differ"
producer="${slicer}/src/sliceProducer/sliceProducer"
analyser="${slicer}/lib/cgBuilder/libcgBuilder.so"
builder="${slicer}/sh/compileAndLinkLL.sh"

# Check for existence.
if [ ! -f ${differ} ]; then
  echo "Differ didn't exist at assumed path."
  exit 1
else
  echo "Differ: ${differ}"
fi
if [ ! -f ${differ} ]; then
  echo "Slice producer didn't exist at assumed path."
  exit 1
else
  echo "Slice producer: ${producer}"
fi
if [ ! -f ${differ} ]; then
  echo "Call graph analyser didn't exist at assumed path."
  exit 1
else
  echo "Call graph analyser: ${analyser}"
fi

# Go back to base.
cd "${base}"

# Set up for building versions.
src="${base}/src"
inc="${base}/include"
buildDir="${base}/sliceBuilds"
projDir="${buildDir}/${projName}"
oldBC="${projDir}/${oldSHA}.bc"
curBC="${projDir}/${curSHA}.bc"

# Build for cur version.
# TODO: MODIFY ME FOR YOUR BUILD.
curCFiles=\
`find "${src}" -type f -name *.c -print`
"${builder}" "${projName}" "${curSHA}" "${buildDir}" "${inc}" ${curCFiles}

# Checkout old version.
# TODO: MODIFY ME FOR YOUR BUILD.
git checkout "${oldSHA}"
oldCFiles=\
`find "${src}" -type f -name *.c -print`
"${builder}" "${projName}" "${oldSHA}" "${buildDir}" "${inc}" ${oldCFiles}

# Reset the directory to the original SHA to let tools and tests run on correct source version.
git checkout "${curSHA}"

# Do the diff.
diffFile="${buildDir}/diff.txt"
"${differ}" "${oldBC}" "${curBC}" > "${diffFile}"
echo "Diff file:"
cat "${diffFile}"

# Do the affected callgraph analysis.
affFile="${buildDir}/aff.txt"
"${opt}" "${curBC}" \
  -load="${analyser}" -cgBuilder \
  -track="${diffFile}" -dest="${affFile}" \
  -calls \
  > /dev/null
echo "Affected file:"
cat "${affFile}"

# Do the affected callgraph analysis.
testFile="${buildDir}/tests.txt"
"${opt}" "${curBC}" \
  -load="${analyser}" -cgBuilder \
  -track="${affFile}" -dest="${testFile}" \
  -tests \
  > /dev/null
echo "Tests file:"
cat "${testFile}"

# Produce the slice tests file.
sliceFile="${src}/tests/testSlice.c"
"${producer}" "${testFile}" "${sliceFile}"
echo "Slice file:"
cat "${sliceFile}"

