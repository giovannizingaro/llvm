#include "llvm/NoCryptoFA/CalcDFG.h"
#include "llvm/NoCryptoFA/All.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/ErrorHandling.h"
#include <llvm/IR/Metadata.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Analysis/Dominators.h>
#include <set>
#include <list>
#include <iostream>
#include <future>
#include <unistd.h>
#include <sys/time.h>
#include "llvm/Support/CommandLine.h"
#include "ForwardAnalysis.h"
#include "BackwardAnalysis.h"
#include "FaultAnalysis.h"

/*
using namespace llvm;
static cl::opt<unsigned int>
SecurityMargin("nocryptofa-security-margin", cl::init(128), cl::ValueRequired,
               cl::desc("NoCryptoFA Security Margin (bits)"));
*/
static cl::opt<bool>
MaskEverything("nocryptofa-mask-everything", cl::init(false), cl::ValueRequired,
               cl::desc("NoCryptoFA Mask Everything"));

char llvm::CalcDFG::ID = 0;
static int __cnt = 0;

CalcDFG* llvm::createCalcDFGPass()
{
    return new CalcDFG();
}

unsigned int CalcDFG::getMSBEverSet_Fault()
{
    return MSBEverSet_Fault;
}
unsigned int CalcDFG::getMSBEverSet()
{
    return MSBEverSet;
}
bool CalcDFG::functionMarked(Function* ptr){

    return (markedfunctions.count(ptr) > 0);
}
void waitTillReady(sem_t *ready){
    if(ready==NULL) return;
    sem_wait(ready);
}
void done(sem_t *ready){
    if(ready==NULL) return;
    sem_post(ready);
}


bool CalcDFG::runOnFunction(llvm::Function& Fun)
{
	if(!Analysis::init(getAnalysis<TaggedData>(),Fun)) return false;
	else markedfunctions.insert(&Fun);
	ForwardAnalysis* forw = new ForwardAnalysis();
	forw->calcAnalysis();
	BackwardAnalysis* back = new BackwardAnalysis();
	back->calcAnalysis();
	FaultAnalysis* fault = new FaultAnalysis();
	fault-> calcAnalysis();
	errs() << "calc pre/post masking needs done\n";
	return false;
}

llvm::NoCryptoFA::InstructionMetadata* CalcDFG::getMD(llvm::Instruction* ptr)
{
	return NoCryptoFA::known[ptr];
}

bool CalcDFG::shouldBeProtected(Instruction* ptr)
{
	NoCryptoFA::InstructionMetadata* md = NoCryptoFA::known[ptr];
	return (MaskEverything && md->hasMetPlaintext) || md->hasToBeProtected_pre || md->hasToBeProtected_post;
	//   return NoCryptoFA::known[ptr]->hasMetPlaintext;
}

unsigned int CalcDFG::getOperandSize(llvm::Instruction* ptr)
{
	return getOperandSize(ptr->getType());
}
 unsigned int CalcDFG::getOperandSize(llvm::Type* t)
{
    if(t->isVoidTy()) return 0;
	while(t->isPointerTy()) {
		t = t->getPointerElementType();
	}
    //TODO: espandere strutture supportate
    int dim = t->getScalarSizeInBits();
    if(dim > 0) return dim;
    dim = t->getPrimitiveSizeInBits();
    if(dim > 0) return dim;
    errs() << "Errore: OperandSize==0 per tipo " << t << "\n";
    return 0;

}


void CalcDFG::getAnalysisUsage(llvm::AnalysisUsage& AU) const
{
	// This is an analysis, nothing is modified, so other analysis are preserved.
	AU.addRequired<TaggedData>();
	AU.setPreservesCFG();
	AU.setPreservesAll();

}

using namespace llvm;


INITIALIZE_PASS_BEGIN(CalcDFG,
                      "CalcDFG",
                      "CalcDFG",
                      false,
                      true)
INITIALIZE_PASS_DEPENDENCY(TaggedData)

INITIALIZE_PASS_END(CalcDFG,
                    "CalcDFG",
                    "CalcDFG",
                    false,
                    true)
