#include "llvm/InstVisitor.h"
#include "llvm/Support/MathExtras.h"

template<unsigned long N>
using BitMatrix = std::vector<std::bitset<N> >;

template<unsigned long N>
using BitColumn = std::bitset<N>;

template<unsigned long N>
BitMatrix<N> ReverseShlByConst(const BitMatrix<N> &M, unsigned ShAmt) {
  unsigned Size = M.size();
  std::vector<std::bitset<N> > Res(Size);
  std::copy(M.begin(), M.begin() + (Size - ShAmt), Res.begin() + ShAmt);
  return Res;
}

template<unsigned long N>
BitMatrix<N> ReverseShrByConst(const BitMatrix<N> &M, unsigned ShAmt) {
  unsigned Size = M.size();
  BitMatrix<N> Res(Size);
  std::copy(M.begin() + ShAmt, M.end(), Res.begin());
  return Res;
}

template<unsigned long N>
BitMatrix<N> operator|(const BitMatrix<N> &M1, const BitMatrix<N> &M2) {
  assert(M1.size() == M2.size());
  BitMatrix<N> Res(M1);
  for (unsigned i = 0, e = M1.size(); i != e; ++i)
    Res[i] |= M2[i];
  return Res;
}

template<unsigned long N>
BitMatrix<N> &operator|=(BitMatrix<N> &This, const BitMatrix<N> &M) {
  for (auto I1 = This.rbegin(), E1 = This.rend(), 
       I2 = M.rbegin(), E2 = M.rend(); I1 != E1 && I2 != E2; ++I1, ++I2)
    *I1 |= *I2;
  return This;
}

template<unsigned long N>
BitMatrix<N> operator&(const BitMatrix<N> &M, uint64_t Value) {
  BitMatrix<N> Res(M);
  const uint64_t One = 1;
  for (unsigned i = 0, e = M.size(); i != e; ++i) {
    if ((Value & (One << i)) == 0)
      Res[e - 1 - i].reset();
  }
  return Res;
}

template<unsigned long N>
BitMatrix<N> operator|(const BitMatrix<N> &M, uint64_t Value) {
  BitMatrix<N> Res(M);
  const uint64_t One = 1;
  for (unsigned i = 0, e = M.size(); i != e; ++i) {
    if (Value & (One << i) == 1)
      Res[e - 1 - i].reset();
  }
  return Res;
}


template<unsigned long N, uint64_t K>
BitMatrix<N> operator&(const BitMatrix<N> &M, const std::bitset<K> &Value) {
  BitMatrix<N> Res(M);
  const uint64_t One = 1;
  for (unsigned i = 0, Size = M.size(); i != Size; ++i) {
    if (!Value.test(i))
      Res[Size - 1 - i].reset();
  }
  return Res;
}


template<unsigned long N>
BitMatrix<N> ReduceAndPad(const BitMatrix<N> &M) {
  assert(M.size());
  BitMatrix<N> Res(M);
  for (unsigned i = 1, e = M.size(); i != e; ++i)
    Res[0] |= Res[i];
  
  std::fill(Res.begin() + 1, Res.end(), Res[0]);
  return Res;
}

template<unsigned long N>
BitMatrix<N> ReduceAndPad(const BitMatrix<N> &M, unsigned DestSize) {
  assert(M.size());
  BitMatrix<N> Res(DestSize);
  for (unsigned i = 0, e = M.size(); i != e; ++i)
    Res[0] |= M[i];
  
  std::fill(Res.begin() + 1, Res.end(), Res[0]);
  return Res;
}


template<unsigned long N>
BitMatrix<N> IntegralReduceAndPad(const BitMatrix<N> &M, unsigned LinLimit) {
  assert(M.size());
  unsigned Size = M.size();
  BitMatrix<N> Res(M.size());

  BitColumn<N> Acc(M[Size - LinLimit]);

  for (unsigned i = Size - LinLimit; i != 0; --i) {
    Acc |= M[i - 1];
    Res[i - 1] |= Acc;
  }
  return Res;
}


class CalcBackwardFAVisitor : public InstVisitor<CalcBackwardFAVisitor> {
public:
  typedef NoCryptoFA::InstructionMetadata InstMetadata;

public:
  void setTargetMetadata(Instruction *TI, InstMetadata *TMD) {
    TargetMD = TMD;
    Target = TI;
  }

  void visitInstruction(Instruction &I) LLVM_OVERRIDE {
    llvm_unreachable("Analysis method unknown!");
  }

  void visitXor(BinaryOperator &I) LLVM_OVERRIDE {
    Value *OtherOp = I.getOperand(0) == Target ? I.getOperand(1)
                                               : I.getOperand(0);

    NoCryptoFA::InstructionMetadata *CurMD = NoCryptoFA::known[&I];

    if (ConstantInt *C = dyn_cast<ConstantInt>(OtherOp)) {
      TargetMD->LinearDeps = CurMD->LinearDeps;
      TargetMD->NonLinearDeps = CurMD->NonLinearDeps;
      return;
    }

    Instruction *OtherI = cast<Instruction>(OtherOp);

    NoCryptoFA::InstructionMetadata *OtherMD = NoCryptoFA::known[OtherI];

    TargetMD->LinearDeps |= CurMD->LinearDeps | OtherMD->LinearDeps;
    TargetMD->NonLinearDeps |= CurMD->NonLinearDeps | OtherMD->NonLinearDeps;
  }

  void visitShl(BinaryOperator &I) LLVM_OVERRIDE {
    if (Target == I.getOperand(1)) return;

    NoCryptoFA::InstructionMetadata *CurMD = NoCryptoFA::known[&I];

    if (ConstantInt *C = dyn_cast<ConstantInt>(I.getOperand(1))) {
      unsigned ShAmt = C->getZExtValue();

      TargetMD->LinearDeps |= ReverseShlByConst(CurMD->LinearDeps, ShAmt);
      TargetMD->NonLinearDeps |= ReverseShlByConst(CurMD->NonLinearDeps, ShAmt);
      return;
    }

    Instruction *OtherI = cast<Instruction>(I.getOperand(1));
    NoCryptoFA::InstructionMetadata *OtherMD = NoCryptoFA::known[OtherI];

    TargetMD->LinearDeps |= CurMD->LinearDeps;

    TargetMD->NonLinearDeps |= CurMD->NonLinearDeps |
                               ReduceAndPad(OtherMD->LinearDeps |
                                            OtherMD->NonLinearDeps);
  }

  void visitLShr(BinaryOperator &I) LLVM_OVERRIDE {
    if (Target == I.getOperand(1)) return;

    NoCryptoFA::InstructionMetadata *CurMD = NoCryptoFA::known[&I];

    if (ConstantInt *C = dyn_cast<ConstantInt>(I.getOperand(1))) {
      unsigned ShAmt = C->getZExtValue();

      TargetMD->LinearDeps |= ReverseShrByConst(CurMD->LinearDeps, ShAmt);
      TargetMD->NonLinearDeps |= ReverseShrByConst(CurMD->NonLinearDeps, ShAmt);
      return;
    }

    Instruction *OtherI = cast<Instruction>(I.getOperand(1));
    NoCryptoFA::InstructionMetadata *OtherMD = NoCryptoFA::known[OtherI];

    TargetMD->LinearDeps |= CurMD->LinearDeps;

    TargetMD->NonLinearDeps |= CurMD->NonLinearDeps |
                               ReduceAndPad(OtherMD->LinearDeps |
                                            OtherMD->NonLinearDeps);
  }

  void visitAShr(BinaryOperator &I) LLVM_OVERRIDE {
    visitLShr(I);
  }

  void visitAnd(BinaryOperator &I) LLVM_OVERRIDE {
    Value *OtherOp = I.getOperand(0) == Target ? I.getOperand(1)
                                               : I.getOperand(0);

    NoCryptoFA::InstructionMetadata *CurMD = NoCryptoFA::known[&I];

    if (ConstantInt *C = dyn_cast<ConstantInt>(OtherOp)) {
      unsigned Value = C->getZExtValue();
      TargetMD->LinearDeps |= CurMD->LinearDeps & Value;
      TargetMD->NonLinearDeps |= CurMD->NonLinearDeps & Value;
      return;
    }

    Instruction *OtherI = cast<Instruction>(OtherOp);
    NoCryptoFA::InstructionMetadata *OtherMD = NoCryptoFA::known[OtherI];
    
    TargetMD->NonLinearDeps |= CurMD->NonLinearDeps | CurMD->LinearDeps |
                               OtherMD->NonLinearDeps | OtherMD->LinearDeps |
                               TargetMD->LinearDeps;
  }

  void visitOr(BinaryOperator &I) LLVM_OVERRIDE {
    Value *OtherOp = I.getOperand(0) == Target ? I.getOperand(1)
                                               : I.getOperand(0);

    NoCryptoFA::InstructionMetadata *CurMD = NoCryptoFA::known[&I];

    if (ConstantInt *C = dyn_cast<ConstantInt>(OtherOp)) {
      unsigned Value = C->getZExtValue();
      TargetMD->LinearDeps |= CurMD->LinearDeps | Value;
      TargetMD->NonLinearDeps |= CurMD->NonLinearDeps | Value;
      return;
    }

    Instruction *OtherI = cast<Instruction>(OtherOp);
    NoCryptoFA::InstructionMetadata *OtherMD = NoCryptoFA::known[OtherI];
    
    TargetMD->NonLinearDeps |= CurMD->NonLinearDeps | CurMD->LinearDeps |
                               OtherMD->NonLinearDeps | OtherMD->LinearDeps |
                               TargetMD->LinearDeps;

  }

  void visitAdd(BinaryOperator &I) LLVM_OVERRIDE {
    Value *OtherOp = I.getOperand(0) == Target ? I.getOperand(1)
                                               : I.getOperand(0);

    NoCryptoFA::InstructionMetadata *CurMD = NoCryptoFA::known[&I];

    if (ConstantInt *C = dyn_cast<ConstantInt>(OtherOp)) {
      unsigned LinLimit = 1 + countTrailingZeros(C->getZExtValue());
      uint64_t Mask = (uint64_t(1) << LinLimit) - 1;

      // Least significant bit is in a linear relation
      TargetMD->LinearDeps |= CurMD->LinearDeps & Mask;
      TargetMD->NonLinearDeps |= CurMD->NonLinearDeps & Mask;
    
      // Other bits are in a non linear relation
      TargetMD->NonLinearDeps |= IntegralReduceAndPad(CurMD->NonLinearDeps |
                                                      CurMD->LinearDeps,
                                                      LinLimit);

      return;
    }

    Instruction *OtherI = cast<Instruction>(OtherOp);
    NoCryptoFA::InstructionMetadata *OtherMD = NoCryptoFA::known[OtherI];

    unsigned LinLimit = 1;
    uint64_t Mask = (uint64_t(1) << LinLimit) - 1;

    // Least significant bit is in a linear relation
    TargetMD->LinearDeps |= CurMD->LinearDeps & Mask;
    TargetMD->NonLinearDeps |= CurMD->NonLinearDeps & Mask;
    
    // Other bits are in a non linear relation
    TargetMD->NonLinearDeps |= IntegralReduceAndPad(CurMD->NonLinearDeps |
                                                    CurMD->LinearDeps |
                                                    OtherMD->NonLinearDeps |
                                                    OtherMD->LinearDeps,
                                                    LinLimit);
  }

  void visitSub(BinaryOperator &I) LLVM_OVERRIDE {
    if (isa<ConstantInt>(I.getOperand(1)))
      llvm_unreachable("We should not be here!");

    Value *OtherOp = I.getOperand(0) == Target ? I.getOperand(1)
                                               : I.getOperand(0);

    NoCryptoFA::InstructionMetadata *CurMD = NoCryptoFA::known[&I];

    if (ConstantInt *C = dyn_cast<ConstantInt>(OtherOp)) {
      unsigned LinLimit = 1 + countTrailingZeros(~ C->getZExtValue());
      uint64_t Mask = (uint64_t(1) << LinLimit) - 1;

      // Least significant bit is in a linear relation
      TargetMD->LinearDeps |= CurMD->LinearDeps & Mask;
      TargetMD->NonLinearDeps |= CurMD->NonLinearDeps & Mask;
    
      // Other bits are in a non linear relation
      TargetMD->NonLinearDeps |= IntegralReduceAndPad(CurMD->NonLinearDeps |
                                                      CurMD->LinearDeps,
                                                      LinLimit);

      return;
    }

    Instruction *OtherI = cast<Instruction>(OtherOp);
    NoCryptoFA::InstructionMetadata *OtherMD = NoCryptoFA::known[OtherI];

    unsigned LinLimit = 1;
    uint64_t Mask = (uint64_t(1) << LinLimit) - 1;

    // Least significant bit is in a linear relation
    TargetMD->LinearDeps |= CurMD->LinearDeps & Mask;
    TargetMD->NonLinearDeps |= CurMD->NonLinearDeps & Mask;
    
    // Other bits are in a non linear relation
    TargetMD->NonLinearDeps |= IntegralReduceAndPad(CurMD->NonLinearDeps |
                                                    CurMD->LinearDeps |
                                                    OtherMD->NonLinearDeps |
                                                    OtherMD->LinearDeps,
                                                    LinLimit);
  }

  void visitGetElementPtrInst(GetElementPtrInst &I) LLVM_OVERRIDE {
    NoCryptoFA::InstructionMetadata *CurMD = NoCryptoFA::known[&I];

    assert(I.getNumIndices() == 2);

    Instruction *Idx = cast<Instruction>(I.getOperand(2));

    assert(Idx == Target);

    TargetMD->LinearDeps |= CurMD->LinearDeps;
    TargetMD->NonLinearDeps |= CurMD->NonLinearDeps;
  }

  void visitLoad(LoadInst &I) LLVM_OVERRIDE {
    NoCryptoFA::InstructionMetadata *CurMD = NoCryptoFA::known[&I];

    BitMatrix<MAX_SUBBITS> Base = (CurMD->LinearDeps | CurMD->NonLinearDeps) &
                                 ~TargetMD->deadBits;

    TargetMD->NonLinearDeps |= ReduceAndPad(Base,
                                            TargetMD->NonLinearDeps.size());
  }

  void visitZExtInst(ZExtInst &I) LLVM_OVERRIDE {
    NoCryptoFA::InstructionMetadata *CurMD = NoCryptoFA::known[&I];

    TargetMD->LinearDeps |= CurMD->LinearDeps;
    TargetMD->NonLinearDeps |= CurMD->NonLinearDeps;
  }

  void visitSExtInst(SExtInst &I) LLVM_OVERRIDE {
    NoCryptoFA::InstructionMetadata *CurMD = NoCryptoFA::known[&I];

    TargetMD->LinearDeps |= CurMD->LinearDeps;
    TargetMD->NonLinearDeps |= CurMD->NonLinearDeps;
  }

  void visitTruncInst(TruncInst &I) LLVM_OVERRIDE {
    NoCryptoFA::InstructionMetadata *CurMD = NoCryptoFA::known[&I];

    TargetMD->LinearDeps |= CurMD->LinearDeps;
    TargetMD->NonLinearDeps |= CurMD->NonLinearDeps;
  }

  void visitStoreInst(StoreInst &I) LLVM_OVERRIDE {}
private:
  Instruction *Target;
  InstMetadata *TargetMD;
};
