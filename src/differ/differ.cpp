// Author: Braedy Kuzma
// This file is a command line driver for a tool that generates differences
// between two DSU-able programs for <insert project name here>
// Much of this source comes from LLVM's llvm-diff tool
// See llvm/tools/llvm-diff/llvm-diff.cpp

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/SourceMgr.h"

#include "differ/DiffLog.h"
#include "differ/DifferenceEngine.h"

using namespace llvm;

// Hide the unecessary options from all the included libraries.
static cl::OptionCategory diffOptions("Differ options");
static cl::opt<std::string> LeftFilename(cl::Positional,
                                         cl::desc("<old ll>"),
                                         cl::Required,
                                         cl::cat(diffOptions));
static cl::opt<std::string> RightFilename(cl::Positional,
                                          cl::desc("<new ll>"),
                                          cl::Required,
                                          cl::cat(diffOptions));

/// Reads a module from a file.  On error, messages are written to stderr
/// and null is returned.
static std::unique_ptr<Module> readModule(LLVMContext &Context,
                                          StringRef Name) {
  SMDiagnostic Diag;
  std::unique_ptr<Module> M = parseIRFile(Name, Diag, Context);
  if (!M)
    Diag.print("differ", errs());
  return M;
}

int main(int argc, char **argv) {
  // Hide the rest of the included options and then parse
  cl::HideUnrelatedOptions(diffOptions);
  cl::ParseCommandLineOptions(argc, argv);

  LLVMContext Context;
  std::unique_ptr<Module> LModule = readModule(Context, LeftFilename);
  std::unique_ptr<Module> RModule = readModule(Context, RightFilename);

  FunctionDiffConsumer Consumer;
  DifferenceEngine Engine(Consumer);

  Engine.diff(LModule.get(), RModule.get());
  Consumer.printFunctions();

  return 0;
}
