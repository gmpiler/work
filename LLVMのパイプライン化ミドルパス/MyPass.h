
#ifndef LLVM_TRANSFORMS_SCALAR_MYPASS_H
#define LLVM_TRANSFORMS_SCALAR_MYPASS_H

#include "llvm/IR/PassManager.h"

namespace llvm {

class Function;

/// Pass to forward loads in a loop around the backedge to subsequent
/// iterations.
struct MyPass : public PassInfoMixin<MyPass> {
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

} // end namespace llvm

#endif // LLVM_TRANSFORMS_SCALAR_LOOPLOADELIMINATION_H
