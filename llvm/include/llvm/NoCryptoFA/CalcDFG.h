#pragma once
#include "TaggedData.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include <llvm/IR/Instruction.h>
#include <map>
#include <list>
#include <set>
#include <bitset>
#include <semaphore.h>

using namespace llvm;

namespace llvm
{
	void initializeCalcDFGPass(PassRegistry& Registry);

	class CalcDFG : public llvm::FunctionPass
	{
		public:
			static char ID;
			CalcDFG() : llvm::FunctionPass(ID) { }
			virtual bool runOnFunction(llvm::Function& Fun);
			virtual NoCryptoFA::InstructionMetadata* getMD(llvm::Instruction* ptr);
                        void setAsTransformed(Function* ptr);
			virtual void getAnalysisUsage(llvm::AnalysisUsage& AU) const;
                        virtual const char* getPassName() const {
                                return "CalcDFG";
                        }
		bool shouldBeProtected(Instruction* ptr);
		bool functionMarked(Function* ptr);
		static unsigned int getOperandSize(llvm::Instruction* ptr);
            	static unsigned int getOperandSize(llvm::Type* t);
            	unsigned int getMSBEverSet();
            	unsigned int getMSBEverSet_Fault();
	};
	CalcDFG* createCalcDFGPass();


}
