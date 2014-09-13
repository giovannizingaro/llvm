#include "llvm/NoCryptoFA/TaggedData.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/ErrorHandling.h"
#include <llvm/IR/Metadata.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Intrinsics.h>
#include <llvm/IR/IntrinsicInst.h>
#include <llvm/IR/Value.h>
#include <llvm/Analysis/Dominators.h>
#include <set>
#include <iostream>
#include <unistd.h>
#include <sys/time.h>
using namespace llvm;
 
std::map<llvm::Instruction*, llvm::NoCryptoFA::InstructionMetadata*> llvm::NoCryptoFA::known = std::map<llvm::Instruction*, llvm::NoCryptoFA::InstructionMetadata*>();
char TaggedData::ID = 212;

TaggedData* llvm::createTaggedDataPass()
{
	return new TaggedData();
}
void TaggedData::markFunction(Function* ptr)
{
	markedfunctions.insert(ptr);
}
bool TaggedData::functionMarked(Function* ptr)
{
	return (markedfunctions.count(ptr) > 0);
}
bool TaggedData::isMarkedAsKey(Instruction* ptr)
{
	return NoCryptoFA::known[ptr]->isAKeyOperation;
}
bool TaggedData::isMarkedAsStatus(Instruction* ptr)
{
	return (ptr->getMetadata("status") != NULL);
}

bool TaggedData::runOnFunction(llvm::Function& Fun)
{
	//std::cerr << "Run on Function";
	hasmd = false;
	for(llvm::Function::iterator FI = Fun.begin(),
	    FE = Fun.end();
	    FI != FE;
	    ++FI) {
		for(llvm::BasicBlock::iterator I = FI->begin(),
		    E = FI->end();
		    I != E;
		    ++I) {
			//I->dump();
			//Cerca il nodo root di plain e key (la call a crypto_{key,plain})
			if(isa<CallInst>(*I)){
			CallInst* call = cast<CallInst>(I);
			//errs() << "Sono qui";
			//Gestione di plain
			if(call->getCalledFunction()->isIntrinsic()){
			if(call->getCalledFunction()->getIntrinsicID()==llvm::Intrinsic::crypto_plain){
				errs() << "Trovata radice plain:";
				call->dump();
				errs() << "Variabile da tracciare:";
				Value* plain = cast<MDNode>(call->getArgOperand(0)) ->getOperand(0);
				if(plain == NULL )  continue;
				plain->dump();
				for(llvm::Instruction::use_iterator I=plain->use_begin(), E = plain->use_end();I != E;++I ) {
					if (Instruction* inst = dyn_cast<Instruction>(*I)) {
                                		inst -> dump();
                                		trackPlain(inst,0);
					}
				}//endfor
			}//Fine gestione del plain

                        //Gestione di chiave
                        if(call->getCalledFunction()->getIntrinsicID()==llvm::Intrinsic::crypto_key){
                                errs() << "Trovata radice key:";
                                call->dump();
                                errs() << "Variabile da tracciare:";
                                Value* key = cast<MDNode>(call->getArgOperand(0)) ->getOperand(0);
                                if(key == NULL ) continue;
				key->dump();
                                for(llvm::Instruction::use_iterator I = key->use_begin(), E = key->use_end();I != E; ++I) {
                                        if (Instruction* inst = dyn_cast<Instruction>(*I)) {
                                                llvm::NoCryptoFA::InstructionMetadata* md = llvm::NoCryptoFA::InstructionMetadata::getNewMD(inst);
						md->isAKeyStart = true;
						inst -> dump();
                                                trackKey(inst);
                                        }//endif
                                }//endfor
                        }//Fine gestione della key

			}else if(isa<llvm::DbgInfoIntrinsic>(I)) continue;//Fine if intrinsic
			}//Fine ricerca nodi root
			if(NoCryptoFA::known.find(I) == NoCryptoFA::known.end()){
			llvm::NoCryptoFA::InstructionMetadata* md = new llvm::NoCryptoFA::InstructionMetadata(I);
                	md->isAKeyOperation = false;
			}
 			//checkMeta(I);
		}
	}
	if(hasmd) {
		markedfunctions.insert(&Fun);
	}
	return false;
}

void TaggedData::trackPlain(llvm::Instruction* plain, long height){
	//plain->dump();
        llvm::NoCryptoFA::InstructionMetadata* md = llvm::NoCryptoFA::InstructionMetadata::getNewMD(plain);
        hasmd = true;
	//if(md->isAKeyOperation) md->isAKeyStart=false; //Needed?
	if(!md->hasMetPlaintext) {
        	md->hasMetPlaintext = true;
        	md->PlaintextHeight = height;
        	for(llvm::Instruction::use_iterator i = plain->use_begin(); i != plain->use_end(); ++i) {
        		if (Instruction* inst = dyn_cast<Instruction>(*i)) {
               	 		trackPlain(inst,(height + 1));
                       		}
               }
        }else
	{
	if(md->PlaintextHeight < height){
	 md->PlaintextHeight = height;
	 for(llvm::Instruction::use_iterator i = plain->use_begin(); i != plain->use_end(); ++i) {
        	 if (Instruction* inst = dyn_cast<Instruction>(*i)) {
         	 	trackPlain(inst,(height + 1));
                         }
               }

	}
	}
}


void TaggedData::trackKey(Instruction* key){
        llvm::NoCryptoFA::InstructionMetadata* md = llvm::NoCryptoFA::InstructionMetadata::getNewMD(key);
        hasmd = true;
        if(!md->isAKeyOperation) {
                md->isAKeyOperation = true;
                for(llvm::Instruction::use_iterator i = key->use_begin(); i != key->use_end(); ++i) {
                        if (Instruction* inst = dyn_cast<Instruction>(*i)) {
                                trackKey(inst);
                        }
                }

	}
}

llvm::NoCryptoFA::InstructionMetadata* TaggedData::getMD(llvm::Instruction* ptr)
{
	return NoCryptoFA::known[ptr];
}

void TaggedData::getAnalysisUsage(llvm::AnalysisUsage& AU) const
{
	// This is an analysis, nothing is modified, so other analysis are preserved.
    AU.setPreservesCFG();
	AU.setPreservesAll();
}

using namespace llvm;

INITIALIZE_PASS_BEGIN(TaggedData,
                      "TaggedData",
                      "TaggedData",
                      true,
                      true)

INITIALIZE_PASS_END(TaggedData,
                    "TaggedData",
                    "TaggedData",
                    true,
                    true)
