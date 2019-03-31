#include "cgBuilder/cgBuilder.h"

#include "llvm/ADT/StringSet.h"
#include "llvm/Analysis/CallGraph.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"

#include <algorithm>
#include <fstream>
#include <memory>
#include <queue>
#include <set>
#include <utility>

#define DEBUG_TYPE "cgBuilder"

// Anonynmous namespace for utility functions.
namespace {

// std::stack can't be indexed. Quick replacement on top of vector.
template <typename T>
class IndexableStack {
public:
  // Default constructor.
  IndexableStack() = default;

  // Push functions.
  void push(const T t) { st.emplace_back(std::move(t)); }

  // Pop functions.
  void pop() { st.pop_back(); }

  // Util functions
  const T &operator[](size_t idx) const { return st[idx]; }
  size_t size() const { return st.size(); }

  // Allow iteration.
  typename std::vector<T>::const_iterator begin() const { return st.begin(); }
  typename std::vector<T>::const_iterator end() const { return st.end(); }

private:
  std::vector<T> st;
};

// Convenience typedefs.
typedef std::pair<const llvm::Function * const, std::unique_ptr<llvm::CallGraphNode>> CGMapElem;
typedef std::reference_wrapper<const CGMapElem> CGMapElemCRef;

void readListFile(const std::string &inFilePath, llvm::StringSet<> &res) {
  // Set up.
  std::string word;
  std::ifstream searchFile(trackList);
  assert(searchFile.good() && "In file started in bad state.");

  // Read words at a time.
  searchFile >> word;
  while (searchFile.good()) {
    res.insert(word);
    searchFile >> word;
  }
}

void trackCallsAux(const llvm::CallGraphNode &work, const llvm::StringSet<> &search,
                   IndexableStack<const llvm::Function *> &callStack,
                   std::set<const llvm::Function *> &seen, llvm::StringSet<> &found) {
  // Get reused values.
  llvm::Function *workFunc = work.getFunction();
  assert(workFunc != nullptr && "workFunc was nullptr");
  const llvm::StringRef &funcName = workFunc->getName();

  llvm::errs() << "Received: " << funcName << '\n';

  // If we were looking for this function, the callstack until now into the list of functions who
  // are affected.
  if (search.count(funcName) > 0) {
    llvm::errs() << funcName << " required work.\n";
    for (const llvm::Function *f : callStack)
      found.insert(f->getName());
  }

  // We've already been here, anything below this is work that's been done already.
  if (seen.count(workFunc) > 0)
    return;

  llvm::errs() << "Continuing with " << funcName << '\n';

  // We've seen this node now. Don't want to reprocess the calls under this.
  seen.emplace(workFunc);

  // Put us in the call stack so our recursive calls know how called them.
  callStack.push(workFunc);

  // Begin recursing.
  for (const llvm::CallGraphNode::CallRecord &record : work) {
    llvm::CallGraphNode *node = record.second;
    node->print(llvm::errs());
    if (node->getFunction() == nullptr)
      continue;
    trackCallsAux(*node, search, callStack, seen, found);
  }

  // Remove us from the call stack.
  callStack.pop();
}

} // End anonymous namespace.


namespace cgBuilder {

char CGBuilderPass::ID = 0;

CGBuilderPass::CGBuilderPass() : ModulePass(ID) {
  llvm::errs() << "CGBUILDERPASS\n";
}

bool CGBuilderPass::runOnModule(llvm::Module &m) {
  bool buildCalls = calls.getValue() == llvm::cl::BOU_TRUE;
  bool buildTests = tests.getValue() == llvm::cl::BOU_TRUE;

  // If we didn't get our options then we can't continue.
  if ((buildCalls && buildTests) || (!buildCalls && !buildTests)) {
    llvm::errs() << "Must set exactly one of --tests or --calls\n";
    return false;
  }

  llvm::errs() << "Starting CGBUILDER pass.\n";

  if (buildCalls)
    trackCalls(m);
  else
    trackTests();

  // Did not change the analysis.
  return false;
}

// Declare we need the call graph analysis to happen before us.
void CGBuilderPass::getAnalysisUsage(llvm::AnalysisUsage &au) const {
  au.addRequired<llvm::CallGraphWrapperPass>();
  au.setPreservesAll();
}

void CGBuilderPass::trackCalls(const llvm::Module &m) {
  // Set up for recursive call.
  // Known.
  llvm::StringSet<> searching;

  // Intermediate building.
  IndexableStack<const llvm::Function *> callStack;
  std::set<const llvm::Function *> seen;

  // Result.
  llvm::StringSet<> foundNames;

  // Get tracking list.
  readListFile(trackList, searching);

  // Mark all input function names as affected.
  for (const llvm::StringMapEntry<char> &s : searching)
    foundNames.insert(s.first());

  // Get callgraph.
  auto &cgwp = getAnalysis<llvm::CallGraphWrapperPass>();
  llvm::CallGraph &cg = cgwp.getCallGraph();

  // Get main CG node.
  const llvm::Function *main = m.getFunction("main");
  assert(main != nullptr && "Didn't find main when searching calls.");
  const llvm::CallGraphNode &mainNode = *cg[main];

  // Begin recursive DFS.
  trackCallsAux(mainNode, searching, callStack, seen, foundNames);

  // Dump output.
  std::ofstream out(dest);
  llvm::errs() << "Found:\n";
  for (const llvm::StringMapEntry<char> &s : foundNames) {
    llvm::errs() << "  " << s.first() << '\n';
    out << s.first().str() << '\n';
  }
}

void CGBuilderPass::trackTests() { }

} // End namespace cgBuilder.

static llvm::RegisterPass<cgBuilder::CGBuilderPass> X(
    "cgBuilder",
    "CGBuilder",
    false /* Only looks at CFG */,
    true /* Analysis Pass */
);

#undef DEBUG_TYPE
