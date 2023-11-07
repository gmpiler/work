#include "llvm/Transforms/Scalar/JumpThreading.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/DenseSet.h"
#include "llvm/ADT/MapVector.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/BlockFrequencyInfo.h"
#include "llvm/Analysis/BranchProbabilityInfo.h"
#include "llvm/Analysis/CFG.h"
#include "llvm/Analysis/ConstantFolding.h"
#include "llvm/Analysis/DomTreeUpdater.h"
#include "llvm/Analysis/GlobalsModRef.h"
#include "llvm/Analysis/GuardUtils.h"
#include "llvm/Analysis/InstructionSimplify.h"
#include "llvm/Analysis/LazyValueInfo.h"
#include "llvm/Analysis/Loads.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/MemoryLocation.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/Analysis/ValueTracking.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/ConstantRange.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/MDBuilder.h"
#include "llvm/IR/Metadata.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/PatternMatch.h"
#include "llvm/IR/ProfDataUtils.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Use.h"
#include "llvm/IR/Value.h"
#include "llvm/InitializePasses.h"
#include "llvm/Pass.h"
#include "llvm/Support/BlockFrequency.h"
#include "llvm/Support/BranchProbability.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils/Cloning.h"
#include "llvm/Transforms/Utils/Local.h"
#include "llvm/Transforms/Utils/SSAUpdater.h"
#include "llvm/Transforms/Utils/ValueMapper.h"
#include "llvm/Transforms/Scalar/MyPass.h"
#include "llvm/IR/IRBuilder.h"
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iterator>
#include <memory>
#include <utility>
#include <iostream>
#include <string>

using namespace llvm;

#define DebugMode 1 // If 1 then on

 bool pipeliningEnabled(Loop *L) {
  auto Enable = getOptionalBoolLoopAttribute(L, "llvm.loop.pipelining.enable");
  if (Enable.has_value()) {
    return *Enable;
  }
  return false;
}

 bool getLoopHeader(BasicBlock *BB, BasicBlock *Header) {
  for (auto *Pred : predecessors(BB)) {
    if (Pred != BB) {
      if (Header) {
        // multiple Header. not supported
        return false;
      }
      Header = Pred;
    }
  }
  if (!Header) {
    // This is not a simplified loop
    return false;
  }
}

 bool getLoopExit(BasicBlock *BB, BasicBlock *LoopExit) {
  for (auto *Succ : successors(BB)) {
    if (Succ != BB) {
      if (LoopExit) {
        // multiple loop exit. not supported
        return false;
      }
      LoopExit = Succ;
    }
  }
  if (!LoopExit) {
    // Single BB infinite loop
    return false;
  }
}

 Use *getUseOfIfCanonicalLoopIVWithUpperBound(Loop *L, BasicBlock *BB) {
  auto *Terminator = dyn_cast<BranchInst>(BB->getTerminator());
  if (!Terminator) {
    return nullptr;
  }
  auto *ICmp = dyn_cast_or_null<ICmpInst>(Terminator->getCondition());
  if (!ICmp) {
    return nullptr;
  }
  if (ICmpInst::ICMP_ULT != ICmp->getPredicate()) {
    return nullptr;
  }
  auto *LHS = dyn_cast<Instruction>(ICmp->getOperand(0));
  auto *RHS = ICmp->getOperand(1);
  if (!LHS) {
    return nullptr;
  }
  if (!L->isLoopInvariant(RHS)) {
    return nullptr;
  }
  auto *IV = L->getCanonicalInductionVariable();
  if (LHS->getOpcode() == Instruction::Add && LHS->getOperand(0) == IV) {
    return &ICmp->getOperandUse(1);
  }
  return nullptr;
}

//use like searchInstruction(Former, Instruction::ICmp)
Instruction *searchInstruction(BasicBlock *BB, unsigned int Opcode) {
  //char *o = const_cast<char *>(Opcode.c_str());
  for (Instruction &I : *BB) {
    if (I.getOpcode() == Opcode) {
      return &I;
    }
  }
  return NULL;
}

PreservedAnalyses MyPass::run(Function &F,
                                         FunctionAnalysisManager &FAM) {
  //解析対象ループの取り出し
  LoopInfo &LI = FAM.getResult<LoopAnalysis>(F);
  std::vector<llvm::BasicBlock *> TargetLoop;
  Value *N;
  for (Loop *L : LI) {
    if (L->getNumBlocks() != 1) { // 今回ループ中のBB数は1と決まっているため
      continue;
    }

    if (pipeliningEnabled(L)) {
      TargetLoop = L->getBlocks();
      Use *UpperBound = getUseOfIfCanonicalLoopIVWithUpperBound(L, TargetLoop[0]);
      if (!UpperBound) {
        continue;
      }
      N = UpperBound->get();
      break;
    } else {
      continue;
    }
  }
  if (TargetLoop.size() == 0) {
    return PreservedAnalyses::all();
  }

  //命令の分割
  std::vector<Instruction *> Form;
  std::vector<Instruction *> Latt;
  for (BasicBlock &BB: F) {
    if (&BB != TargetLoop[0]) {
      continue;
    }
     bool key = true;
    for (Instruction &I : BB) {
      if (I.getOpcode() != Instruction::FAdd && key) {
        Form.push_back(&I);
        continue;
      }
      if (key) {
        Form.push_back(&I);
        key = false;
        continue;
      }
      Latt.push_back(&I);
    }
    break;
  }
  std::cout << "\n\nForm:\n";
  for (auto a: Form) {
    if(DebugMode == 1) a->print(dbgs());
    if(DebugMode == 1) std::cout << "\n";
  }
  std::cout << "\n\nLatt:\n";
  for (auto a : Latt) {
    if(DebugMode == 1) a->print(dbgs());
    if(DebugMode == 1) std::cout << "\n";
  }

  //プロローグ、前後半、エピローグの用意
  LLVMContext &Ctx = F.getContext();
  IRBuilder<> Builder(Ctx);
  BasicBlock *Prologue = BasicBlock::Create(Ctx, "Prologue", &F);
  BasicBlock *Former = BasicBlock::Create(Ctx, "Former", &F);
  BasicBlock *Latter = BasicBlock::Create(Ctx, "Latter", &F);
  BasicBlock *Epilogue = BasicBlock::Create(Ctx, "Epilogue", &F);

  //ループのヘッダを取り出す
  BasicBlock *Header = nullptr;
  for (auto *Pred : predecessors(TargetLoop[0])) {
    if (Pred != TargetLoop[0]) {
      if (Header) {
        // multiple Header. not supported
        return PreservedAnalyses::all();
      }
      Header = Pred;
    }
  }
  if (!Header) {
    // This is not a simplified loop
    return PreservedAnalyses::all();
  }
  //getExit
  BasicBlock *LoopExit = nullptr;
  for (auto *Succ : successors(TargetLoop[0])) {
    if (Succ != TargetLoop[0]) {
      if (LoopExit) {
        // multiple loop exit. not supported
        return PreservedAnalyses::all();
      }
      LoopExit = Succ;
    }
  }
  if (!LoopExit) {
    // Single BB infinite loop
    return PreservedAnalyses::all();
  }
  // N = N - 1
  Builder.SetInsertPoint(Header->getTerminator());
  Constant *One = Builder.getInt1(1);
  Constant *One2 = Builder.getInt64(1);
  Constant *Zero = Builder.getInt64(0);
  for (Instruction *I : Latt) {
    if (I->getOpcode() == Instruction::ICmp) {
      Builder.CreateSub(I->getOperand(1), One2);
      Instruction *Nmin1;
      Nmin1 = searchInstruction(Header, Instruction::Sub);
      I->setOperand(1, Nmin1);
    }
  }
  Instruction *HBr = searchInstruction(Header, Instruction::Br);
  HBr->setOperand(0, Prologue);
  std::cout << "\n[HEADER]\n";
  Header->print(dbgs());
  std::cout << std::endl;


  //命令のコピー
  ValueToValueMapTy VMapP, VMapF, VMapL, VMapE;
  //プロローグ
  Instruction *CIP;
  for (auto a : Form) {
    CIP = a->clone();
    CIP->insertInto(Prologue, Prologue->end());
    VMapP[a] = CIP;
  }
  if(DebugMode == 1) std::cout << "[PROLOGUE]\n";
  if(DebugMode == 1) Prologue->print(dbgs());
  //前半
  Instruction *CIF;
  for (auto a : Latt) {
    CIF = a->clone();
    CIF->insertInto(Former, Former->end());
    VMapF[a] = CIF;
  }
  if(DebugMode == 1) std::cout << "[FORMER]";
  if(DebugMode == 1) Former->print(dbgs());
  //後半
  Instruction *CIL;
  for (auto a : Form) {
    CIL = a->clone();
    CIL->insertInto(Latter, Latter->end());
    VMapL[a] = CIL;
  }
  if(DebugMode == 1) std::cout << "[LATTER]\n";
  if(DebugMode == 1) Latter->print(dbgs());
  //エピローグ
  Instruction *CIE;
  for (auto a : Latt) {
    CIE = a->clone();
    CIE->insertInto(Epilogue, Epilogue->end());
    VMapE[a] = CIE;
  }
  if(DebugMode == 1) std::cout << "[EPILOGUE]\n";
  if(DebugMode == 1) Epilogue->print(dbgs());

  //命令の編集
  //プロローグ
  Builder.SetInsertPoint(Prologue, Prologue->end());
  Builder.CreateBr(Former);
  std::cout << "[PROLOGUEAFTER]\n";
  Prologue->print(dbgs());
  for (Instruction &I : *Prologue) {
    RemapInstruction(&I, VMapP);
  }
  for (PHINode &PHI : Prologue->phis()) {
    PHI.removeIncomingValue(Header, false);
    PHI.removeIncomingValue(TargetLoop[0], false);
    PHI.addIncoming(Zero, Header);
  }
  //前半
  BasicBlock::iterator p;
  auto t = Former->getTerminator();
  t->removeFromParent();
  p = t->insertInto(Latter, Latter->end());
  auto &b = Former->back();
  b.removeFromParent();
  b.insertInto(Latter, p);
  Builder.SetInsertPoint(Former, Former->end());
  Builder.CreateBr(Latter);
  Builder.SetInsertPoint(Former, Former->begin());

  //マッピング処理
  ValueToValueMapTy VMap2, VMap3, VMap4, VMap, VMapP2F, VMap5;
  //Formerの前半命令列とFormを一対一対応
  int i = 0;
  std::vector<llvm::PHINode *> PHIS;
  for (Instruction &I : *Latter) {
    Type *Ty = I.getType();
    if (Ty->isVoidTy() == false) {
      PHINode *PHI = Builder.CreatePHI(Ty, 2);
      PHI->addIncoming(&I, Latter);
      PHIS.push_back(PHI);
    }
  }
  i = 0;
  for (Instruction &I : *Prologue) {
    if (I.getType()->isVoidTy() == false) {
      PHIS[i]->addIncoming(&I, Prologue);
      i++;
      continue;
    }
    if (I.getType()->isVoidTy() == true) {
      PHIS[i]->addIncoming(One, Prologue);
      i++;
    }
  }
  i = 0;
  for (Instruction *I : Form) {
    if (i < PHIS.size()) {
      VMap2[I] = PHIS[i];
      I->print(dbgs());
      std::cout << " is mapped to ";
      PHIS[i]->print(dbgs());
      std::cout << std::endl;
      i++;
    }
  }
  for (Instruction &I : *Former) {
    RemapInstruction(&I, VMap2);
  }
  //Formerの後半命令列とLattを一対一対応
  i = 0;
  for (Instruction &I : *Former) {
    if (I.getOpcode() != Instruction::PHI) {
      VMap3[Latt[i]] = &I;
      Latt[i]->print(dbgs());
      std::cout << " is mapped to ";
      I.print(dbgs());
      std::cout << std::endl;
      i++;
    }
  }
  for (Instruction &I : *Former) {
    RemapInstruction(&I, VMap3);
  }
  //後半
  Builder.SetInsertPoint(Latter, Latter->end());
  //Instruction *LICmp = searchInstruction(Latter, Instruction::ICmp);
  //Instruction *LBr = searchInstruction(Latter, Instruction::Br);
  //LBr->setOperand(0, LICmp);
  //端数調整
  for (Instruction &I : *Latter) {
    if (I.getOpcode() != Instruction::Br) RemapInstruction(&I, VMapL);
    if (I.getOpcode() != Instruction::Br && I.getOpcode() != Instruction::PHI) RemapInstruction(&I, VMap3);
    if (I.getOpcode() == Instruction::Br){
      for (Instruction &II : *Latter) {
        if (II.getOpcode() == Instruction::ICmp) I.setOperand(0, &II); 
        I.setOperand(2, Former);
        I.setOperand(1, Epilogue);
      }
    }
  }

  //PHINode処理, phiの2つめのvalue(1)よりvalueを取り出し、VMapL[value]に入れると%41が帰ってくるのでそれを%44の[%44, %43]に入れたい Prologueのヘッダを取り出す
  for (PHINode &PHI : Latter->phis()) {
    Value *V = PHI.getIncomingValueForBlock(TargetLoop[0]);
    auto val = VMapL[V];
    if (val == nullptr) {
      val = VMapF[V];
    }
    PHI.removeIncomingValue(Header, false);
    PHI.removeIncomingValue(TargetLoop[0], false);
    PHI.addIncoming(val, Former);
  }
  std::cout << "[FORMERAFTER]";
  Former->print(dbgs());
  std::cout << "[LATTERAFTER]\n";
  Latter->print(dbgs());
  // Builder.CreateCondBr(Condition, Former, Epilogue);
  //エピローグ
  Builder.SetInsertPoint(Epilogue, Epilogue->end());
  i = 0;
  for (Instruction &I : *Former) {
    if (I.getOpcode() != Instruction::PHI && (I.getType()->isVoidTy() == false)) {
      VMapP2F[Latt[i]] = &I;
      i++;
    }
  }
  std::vector<Instruction *> vec2;
  for (Instruction &I : *Prologue) {
    if (I.getOpcode() != Instruction::PHI &&
        (I.getType()->isVoidTy() == false)) {
      vec2.push_back(&I);
    }
  }
  i = 0; 
  /*
  for (Instruction &I : *Latter) {
    if (I.getOpcode() != Instruction::PHI &&
        (I.getType()->isVoidTy() == false)) {
      VMap5[vec2[i]] = &I;
      i++;
    }
  }*/
  //端数調整
  ValueToValueMapTy Map;
  for (Instruction &I : *Prologue) {
    if (I.getOpcode() == Instruction::PHI) {
      for (Instruction &II : *Former) {
        if (II.getOpcode() == Instruction::Add) {
          Map[&I] = &II;
        }
      }
    } 
  }
  for (Instruction &I : *Epilogue) {
    RemapInstruction(&I, VMapE);
    I.print(dbgs());
    std::cout << std::endl;
    RemapInstruction(&I, VMapL);
    I.print(dbgs());
    std::cout << std::endl;
    RemapInstruction(&I, VMapP2F);
    I.print(dbgs());
    std::cout << std::endl;
    RemapInstruction(&I, Map);
    I.print(dbgs());
    std::cout << std::endl;
  }
  Instruction *temp;
  for (Instruction &I : *Epilogue) {
    if (I.getOpcode() == Instruction::Br) {
      temp = &I;
    }
  }
  temp->eraseFromParent();
  Builder.SetInsertPoint(Epilogue, Epilogue->end());
  Builder.CreateBr(LoopExit);
  std::cout << "[EPILOGUEAFTER]\n";
  Epilogue->print(dbgs());

  //元のブロックを消す
  TargetLoop[0]->eraseFromParent();
    
  F.print(dbgs()); //現時点でのLLVM IR全体をプリント
  return PreservedAnalyses::none();
}