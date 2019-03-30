#ifndef SLICER_CGBUILDER_H
#define SLICER_CGBUILDER_H

#include "llvm/Pass.h"
#include "llvm/IR/Module.h"

namespace cgBuilder {

class CGBuilderPass : public llvm::ModulePass {
public:
  static char ID;

  CGBuilderPass();

  bool runOnModule(llvm::Module &M) override;

  void getAnalysisUsage(llvm::AnalysisUsage &analysisUsage) const override;

};

} // End namespace cgBuilder


#endif // SLICER_CGBUILDER_H
