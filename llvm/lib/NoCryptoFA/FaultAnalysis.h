#ifndef ANALYSIS
#define ANALYSIS
#include "Analysis.h"
#endif

namespace llvm
{
namespace NoCryptoFA{
class FaultAnalysis : public Analysis {

public:
FaultAnalysis(): Analysis() {}

	void calcAnalysis(){
		set<Instruction*> vulnerableBottom;
		list<pair<int,Instruction*> > sortedList;
		sortedList.insert(sortedList.begin(),candidateVulnerablePointsCT.begin(),candidateVulnerablePointsCT.end());

		lookForMostVulnerableInstructionRepresentingTheEntireUserKey(sortedList,&vulnerableBottom,&NoCryptoFA::InstructionMetadata::isVulnerableBottomSubKey);

		vector<bitset<MAX_KEYBITS> > post_subkeytokey = assignKeyOwn<MAX_SUBBITS>(vulnerableBottom,&NoCryptoFA::InstructionMetadata::post_own,&MSBEverSet,"vuln_bottom");

		set<Instruction*> firstVulnerableUses = set<Instruction*>();
		for(Instruction * p : vulnerableBottom) {
			for(auto u = p->use_begin(); u != p->use_end(); ++u) {
				Instruction* Inst = dyn_cast<Instruction>(*u);
				firstVulnerableUses.insert(Inst);
			}
		}

		runBatched(firstVulnerableUses, [this](Instruction * p,long batchn)->bool {calcFAKeyBackProp(p);return false;});

	std::map<std::bitset<MAX_OUTBITS>, unsigned > EquivocationMap; 
	runBatched(firstVulnerableUses, [&EquivocationMap, this](Instruction *p, long batchn)->bool {
		llvm::NoCryptoFA::InstructionMetadata*md = getMD(p);
		if (md->EquivocationCount.size() > 0)
			return false;
		md->EquivocationCount.resize(md->out_hit.size());
		md->EquivocationCountSet = false;

		for (const std::bitset<MAX_OUTBITS> &o : md->out_hit)
			++EquivocationMap[o];

		for (auto I = p->op_begin(), E = p->op_end(); I != E; ++I)
			if (Instruction *Instr = dyn_cast<Instruction>(*I))
				toBeVisited.insert(Instr);

		return false;
	});

	runBatched(firstVulnerableUses, [&EquivocationMap, this](Instruction *p, long batchn)->bool {
		llvm::NoCryptoFA::InstructionMetadata*md = getMD(p);
		if (md->EquivocationCountSet) return false;

		for (unsigned i = 0, e = md->out_hit.size(); i != e; ++i)
			md->EquivocationCount[i] = EquivocationMap[md->out_hit[i]];

	md->EquivocationCountSet = true;

	for (auto I = p->op_begin(), E = p->op_end(); I != E; ++I)
		if (Instruction *Instr = dyn_cast<Instruction>(*I))
			toBeVisited.insert(Instr);

	return false;
	});

	runBatched(keyStarts /*cipherOutValues*/, [post_subkeytokey,this](Instruction * p,long batchn)->bool {
		llvm::NoCryptoFA::InstructionMetadata*md = getMD(p);
		if(md->NonLinearKeyDeps.size() > 0){return false;}
		md->LinearKeyDeps.resize(md->LinearDeps.size());
		md->NonLinearKeyDeps.resize(md->NonLinearDeps.size());
		for(unsigned long i =0;i< md->LinearDeps.size();i++){
			std::bitset<MAX_KEYBITS> &Col = md->LinearKeyDeps[i];
			for(unsigned long j =0;j< MAX_SUBBITS;j++){
				if(md->LinearDeps[i][j]) {
					Col |= post_subkeytokey[j];
				}
			}
		}

		for(unsigned long i =0;i< md->NonLinearDeps.size();i++){
			std::bitset<MAX_KEYBITS> &Col = md->NonLinearKeyDeps[i];
			for(unsigned long j =0;j< MAX_SUBBITS;j++){
				if(md->NonLinearDeps[i][j]) {
					Col |= post_subkeytokey[j];
				}
			}
		}

		calcStatistics<MAX_SUBBITS,MAX_KEYBITS>(md->NonLinStats, md->NonLinearDeps,md->NonLinearKeyDeps,1);
                calcStatistics<MAX_SUBBITS,MAX_KEYBITS>(md->NonLinStatsByte, md->NonLinearDeps,md->NonLinearKeyDeps,8);
		for(llvm::Instruction::use_iterator it = p->use_begin(); it != p->use_end(); ++it) {
			if(Instruction* _it = dyn_cast<Instruction>(*it)) {
				toBeVisited.insert(_it);
			}
		}

		return false;
	});

		

	}

void calcFAKeyBackProp(Instruction *ptr)
{
	NoCryptoFA::InstructionMetadata* md = NoCryptoFA::known[ptr];
	if(!md->isAKeyOperation) {return;}
	bool changed = false;
  vector<bitset<MAX_SUBBITS> > OldLin = md->LinearDeps;
  vector<bitset<MAX_SUBBITS> > OldNonLin = md->NonLinearDeps;

  ClearMatrix<MAX_SUBBITS>(md->LinearDeps);
  ClearMatrix<MAX_SUBBITS>(md->NonLinearDeps);

	for (llvm::Instruction::op_iterator it = ptr->op_begin(); it != ptr->op_end(); ++it) {
		if(Instruction* _it = dyn_cast<Instruction>(*it)) {
			NoCryptoFA::InstructionMetadata* opmd = NoCryptoFA::known[_it];
			if(opmd->post_own.count() > 0) {
				md->post_FirstToMeetKey = true;
                /*for(unsigned int i = 0; i < md->post.size(); i++) {
					md->post[i] = md->post[i] | opmd->post_own; // DIAGONALE, non blocchettino!
                }*/
	setDiagonal<MAX_SUBBITS>(md->LinearDeps,opmd->post_own);
			}
		}
	}
    CalcBackwardFAVisitor cbv;
	for(llvm::Instruction::use_iterator it = ptr->use_begin(); it != ptr->use_end(); ++it) {
		if(Instruction* _it = dyn_cast<Instruction>(*it)) {
      cbv.setTargetMetadata(ptr, md);
      cbv.visit(_it);
		}
	}

    if(!areVectorOfBitsetEqual<MAX_SUBBITS>(OldLin, md->LinearDeps) ||
       !areVectorOfBitsetEqual<MAX_SUBBITS>(OldNonLin, md->NonLinearDeps)) {
      changed = true;
    }
	if(changed) {
		for(llvm::Instruction::op_iterator it = ptr->op_begin(); it != ptr->op_end(); ++it) {
			if(Instruction* _it = dyn_cast<Instruction>(*it)) {
				toBeVisited.insert(_it);
			}
		}
	}

}




};

}
}
