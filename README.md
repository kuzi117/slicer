# Slicer
Slicer is a tool for reducing the size each of your test suite runs.

## Contents
This repository builds three objects and contains two scripts.

##### Built objects
  1. differ - A tool for finding function differences between two modules based on llvm-dif.
  1. sliceProducer - A tool for producing a slice of tests for a CuTest enabled project.
  1. libcgBuilder - A shared object containing passes for use with opt. These passes can be used to transform a list of differences into a list of test cases to be run.
  
##### Intermediate scripts
  1. sh/compileAndLinkLL.sh - A script that contains a rudimentary system for compiling and linking versioned llvm modules from c files.
     - Usage: compileAndLinkLL.sh \<name\> \<version\> \<destination dir\> \<include dir\> \<... .c to compile ...\>
  1. sh/setupTests.sh - A script that produces a slice of tests in a git repository. This is meant to be used as part of a Travis CI build. 
     - Usage: setupTests.sh
        - Environment variable `since` must be set to the has to compare against.
        - After initial setup, dropping this in your base directory and executing it will automatically download and build slicer, then use it to compare the current commit against another.
     - Inital setup: There are two find commands in the script labeled `TODO`. These must be modified to find your project's files test and source files. 
     - Assumptions: This script assumes that it is in your base directory, which contains a src and include directory.
 
 ## Build instructions (linux only)
  1. `cd <base directory>`
  1. `mkdir build`
  1. `cd build`
  1. `cmake .. && make -j4`

## Replication
  1. Install llvm and clang 8 from http://apt.llvm.org/
  1. `git clone https://github.com/kuzi117/SlicerExampleProject.git`
  1. `export since=e3db60d02d36567b559477a5af1c447baa4b2368`
  1. `export LLVM_BIN=$(dirname $(which opt))`
  1. `./setupTests.sh`
  1. `mkdir build`
  1. `cd build`
  1. `cmake .. && make -j4`
  1. `./src/runTests`
