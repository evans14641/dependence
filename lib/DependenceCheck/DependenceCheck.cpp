/**
 * \file DependenceCheck.cpp
 * Author: Markus Kusano
 * Date: 2013-11-29
 *
 * Dependence (control and data) checker.
 *
 * Much of the structure of this code is copied from
 * llvm/lib/Analysis/MemDepPrinter.cpp
 */
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/MemoryDependenceAnalysis.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/PostDominators.h"

#include "DataDependence.h"
#include "ControlDependence.h"

using namespace llvm;

// enable debugging output
//#define MK_DEBUG

// In the future this might need to be non-anonymous depending on how we want
// to query this information
namespace {
  // This pass is a module pass for now. I chose this with the idea that the
  // pass could be asked the question: "What are the dependencies of this
  // instruction?"
  struct DependenceCheck : public ModulePass {
    // Default constructor
    DependenceCheck();

    // DataDependence Object. Holds and calculates the data dependency information
    DataDependence DataDep;

    // ControlDependence object. Holds and calculates the control dependency
    // information
    ControlDependence ControlDep;

    static char ID; // pass ID

    virtual bool runOnModule(Module &M);


    virtual void getAnalysisUsage(AnalysisUsage &AU) const;
    void print(raw_ostream &OS, const Module *m = 0) const;

  };

  bool DependenceCheck::runOnModule(Module &M) {
#ifdef MK_DEBUG
    errs() << "[DEBUG] DependenceCheck::runOnModule()\n";
#endif

    AliasAnalysis &AA = getAnalysis<AliasAnalysis>();

    // Iterate over all instructions and get the memory dependence information.
    // TODO: This could be scaled back to only search for one instruction. The
    // efficiency of this depends on if MemoryDependenceAnalysis is also doing
    // an on-demand calculation of dependence
    for (auto mi = M.begin(); mi != M.end(); ++mi) {
      // skip external functions
      if (mi->isDeclaration())
        continue;

      // Since MemoryDependenceAnalysis is a function pass, we need to pass the
      // current function we are examining to the getAnalysis() call.
      MemoryDependenceAnalysis &MDA = getAnalysis<MemoryDependenceAnalysis>(*mi);
      PostDominatorTree &PDT = getAnalysis<PostDominatorTree>(*mi);

      DataDep.getDataDependencies(*mi, MDA, AA);

      ControlDep.getControlDependencies(*mi, PDT);

    } // end for (module::iterator)


    // Nothing modified in IR
    return false;
  }

  void DependenceCheck::print(raw_ostream &OS, const Module *m) const {
    // dump the contents of the local Deps_ map
    OS << "Local Dependence map size: " << DataDep.LocalDeps_.size() << '\n';
    for (auto i = DataDep.LocalDeps_.begin(); i != DataDep.LocalDeps_.end(); ++i) {
      assert(i->first && "NULL inst found");
      OS << "Instruction: " << *(i->first); 
      OS << "\n    has dependence\n";
      OS << "    with instruction " << *(i->second.DepInst_) << '\n';
      OS << "    of type " << DataDependence::depTypeToString(i->second.Type_) << '\n';
    }
    OS << "Non-Local Dependence map size: " << DataDep.NonLocalDeps_.size() << '\n';
    for (auto i = DataDep.NonLocalDeps_.begin(); i != DataDep.NonLocalDeps_.end(); ++i) {
      assert(i->first && "NULL inst found");
      OS << "Instruction: " << *(i->first) 
         << "\n    has non local dependence(s) with:\n";
      for (auto j = i->second.begin(); j != i->second.end(); ++j) {
        assert(j->getAddress());
        OS << "    Address: " << *(j->getAddress()) << '\n';
      }
    }

    // create a dot file for the CDG
    ControlDep.toDot(std::string(""));

    // dump the contents of the control dependencies
    for (auto i = ControlDep.controlDeps_.begin(), 
        ei = ControlDep.controlDeps_.end(); i != ei; ++i) {
      std::set<BasicBlock *> depSet;
      depSet = i->second;

      OS << "BasicBlock: " << *(i->first)
         << "Is dependent on:\n";
      for (auto j = depSet.begin(), ej = depSet.end(); j != ej; ++j) {
        // *j is a pointer to a BasicBlock, **j is a BasicBlock
        OS << **j << '\n';
      }
    }
  }

  DependenceCheck::DependenceCheck() : ModulePass(ID) { 
    initializeMemoryDependenceAnalysisPass(*PassRegistry::getPassRegistry());
  }

  void DependenceCheck::getAnalysisUsage(AnalysisUsage &AU) const {
    // For memory dependence analysis
    AU.addRequired<AliasAnalysis>();
    AU.addRequired<MemoryDependenceAnalysis>();

    // For control dependence analysis
    AU.addRequired<PostDominatorTree>();

    AU.setPreservesAll();
  }

} // end namespace

char DependenceCheck::ID = 0;
static RegisterPass<DependenceCheck> X("depcheck", "check control and data dependencies of instructions", 
    true, /* does not modify CFG */
    true); /* analysis pass */
