#include "differ/DiffConsumer.h"

using namespace llvm;

void FunctionDiffConsumer::markDifference() {
  if (contexts.empty()) return;
  for (SmallVectorImpl<DiffContext>::iterator
         I = contexts.begin(), E = contexts.end(); I != E; ++I) {
    I->Differences = true;
  }
}

void FunctionDiffConsumer::enterContext(Value *L, Value *R) {
  contexts.push_back(DiffContext(L, R));
}

void FunctionDiffConsumer::exitContext() {
  DiffContext &back = contexts.back();
  if (back.IsFunction && back.Differences) {
    functionNames.push_back(back.L->getName());
  }
  contexts.pop_back();
}

void FunctionDiffConsumer::log(StringRef text) {
  markDifference();
  //outs() << "LOG TEXT: " << text << "\n";
}

void FunctionDiffConsumer::logf(const LogBuilder &log) {
  markDifference();
  // outs() << "LOGF TEXT: " << log.getFormat()
  //        << "; NUM ARGS: " << log.getNumArguments() << "\n";
}

void FunctionDiffConsumer::logd(const DiffLogBuilder &log) {
  markDifference();
  // outs() << "LOGD COUNT: " << log.getNumLines() << "\n";
}

void FunctionDiffConsumer::printFunctions() {
  for(SmallVectorImpl<StringRef>::iterator I = functionNames.begin(),
      E = functionNames.end(); I != E; ++I) {
    outs() << *I << "\n";
  }
}
