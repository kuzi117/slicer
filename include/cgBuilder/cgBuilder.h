#ifndef SLICER_CGBUILDER_H
#define SLICER_CGBUILDER_H

#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/CommandLine.h"

static llvm::cl::OptionCategory cgBuilderOpts("CGBuilder options.");
static llvm::cl::opt<std::string> trackList(
    "track",
    llvm::cl::desc("Specify the list of func names that should be tracked for calls."),
    llvm::cl::value_desc("filename"),
    llvm::cl::cat(cgBuilderOpts),
    llvm::cl::Required // I don't like that this is "Required", I want something
    // more like "required if dsutrace is specified"
    // For now this is sufficient though, I don't expect anyone
    // to load the plugin unless they want this functionality
    // in which case it actually IS required
);

static llvm::cl::opt<std::string> dest(
    "dest",
    llvm::cl::desc("Specify the destination for the result list."),
    llvm::cl::value_desc("filename"),
    llvm::cl::cat(cgBuilderOpts),
    llvm::cl::Required // Same as above, this shouldn't be "Required"
);

static llvm::cl::opt<llvm::cl::boolOrDefault> calls(
    "calls",
    llvm::cl::desc("Build a list of functions affected by changes to the input list."),
    llvm::cl::value_desc(""),
    llvm::cl::cat(cgBuilderOpts),
    llvm::cl::Optional,
    llvm::cl::ValueDisallowed
);

static llvm::cl::opt<llvm::cl::boolOrDefault> tests(
    "tests",
    llvm::cl::desc("Build a list of tests that call the input list."),
    llvm::cl::value_desc("filename"),
    llvm::cl::cat(cgBuilderOpts),
    llvm::cl::Optional,
    llvm::cl::ValueDisallowed
);

namespace cgBuilder {

class CGBuilderPass : public llvm::ModulePass {
public:
  static char ID;

  CGBuilderPass();

  bool runOnModule(llvm::Module &m) override;

  void getAnalysisUsage(llvm::AnalysisUsage &analysisUsage) const override;

private:
  void trackCalls(const llvm::Module &m);
  void trackTests();

};

} // End namespace cgBuilder


#endif // SLICER_CGBUILDER_H
