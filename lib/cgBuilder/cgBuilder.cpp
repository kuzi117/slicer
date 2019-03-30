#include "cgBuilder/cgBuilder.h"

#include "llvm/Analysis/CallGraph.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"

#include <set>

#define DEBUG_TYPE "cgBuilder"

namespace cgBuilder {

char CGBuilderPass::ID = 0;

CGBuilderPass::CGBuilderPass() : ModulePass(ID) {
  llvm::errs() << "CGBUILDERPASS\n";
}

bool CGBuilderPass::runOnModule(llvm::Module &M) {
  llvm::errs() << "Starting CGBUILDER pass.\n";
}

// Declare we need the call graph analysis to happen before us.
void CGBuilderPass::getAnalysisUsage(llvm::AnalysisUsage &au) const {
  au.addRequired<llvm::CallGraphWrapperPass>();
  au.setPreservesAll();
}

} // End namespace cgBuilder.

static llvm::RegisterPass<cgBuilder::CGBuilderPass> X(
    "cgBuilder",
    "Tries to CG build or something.",
    false /* Only looks at CFG */,
    true /* Analysis Pass */
);

#undef DEBUG_TYPE
