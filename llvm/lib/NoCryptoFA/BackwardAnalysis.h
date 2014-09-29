#ifndef ANALYSIS
#define ANALYSIS
#include "Analysis.h"
#endif

namespace llvm
{
namespace NoCryptoFA{
class BackwardAnalysis : public Analysis {

public:
set<Instruction*> vulnerableBottom;
list<pair<int,Instruction*> > sortedList;
set<Instruction*> firstVulnerableUses;

void initBackward(){
		sortedList.insert(sortedList.begin(),candidateVulnerablePointsCT.begin(),candidateVulnerablePointsCT.end());
		lookForMostVulnerableInstructionRepresentingTheEntireUserKey(sortedList,&vulnerableBottom,&NoCryptoFA::InstructionMetadata::isVulnerableBottomSubKey);

		firstVulnerableUses = set<Instruction*>();
		for(Instruction * p : vulnerableBottom) {
			for(auto u = p->use_begin(); u != p->use_end(); ++u) {
				Instruction* Inst = dyn_cast<Instruction>(*u);
				firstVulnerableUses.insert(Inst);
			}
		}
}

//public:
BackwardAnalysis(): Analysis() {}

	void calcAnalysis(){

		initBackward();
			vector<bitset<MAX_KEYBITS> >  post_subkeytokey = assignKeyOwn<MAX_SUBBITS>(vulnerableBottom,&NoCryptoFA::InstructionMetadata::post_own,&MSBEverSet,"vuln_bottom");

		runBatched(firstVulnerableUses, [this](Instruction * p,long batchn)->bool {calcPost(p);return false;});
		errs() << "POST-subkeys prop done\n";

		runBatched(keyStarts, [post_subkeytokey,this](Instruction * p,long batchn)->bool {
			llvm::NoCryptoFA::InstructionMetadata*md = getMD(p);
			if(md->post_keydep.size() > 0){return false;}
			md->post_keydep.resize(md->post.size());
			for(unsigned long i =0;i< md->post.size();i++){
				bitset<MAX_KEYBITS> kb=0;
				for(unsigned long j =0;j< MAX_SUBBITS;j++){
					if(md->post[i][j]){
					kb |= post_subkeytokey[j];
					}
				}
				md->post_keydep[i] = kb;
			}

			for(llvm::Instruction::use_iterator it = p->use_begin(); it != p->use_end(); ++it) {
				if(Instruction* _it = dyn_cast<Instruction>(*it)) {
					toBeVisited.insert(_it);
				}
			}
		return false;
		});

		runBatched(firstVulnerableUses, [this](Instruction * p,long batchn)->bool {checkPost_masking(p);return false;});
		errs() << "\nPOST masking done\n"; 

	}

void calcPost(Instruction* ptr)
{
	NoCryptoFA::InstructionMetadata* md = NoCryptoFA::known[ptr];
	if(!md->isAKeyOperation) {return;}
	bool changed = false;
    vector<bitset<MAX_SUBBITS> > oldPost = md->post;
    ClearMatrix<MAX_SUBBITS>(md->post);
	for(llvm::Instruction::op_iterator it = ptr->op_begin(); it != ptr->op_end(); ++it) {
		if(Instruction* _it = dyn_cast<Instruction>(*it)) {
			NoCryptoFA::InstructionMetadata* opmd = NoCryptoFA::known[_it];
			if(opmd->post_own.count() > 0) {
				md->post_FirstToMeetKey = true;
                setDiagonal<MAX_SUBBITS>(md->post,opmd->post_own);
			}
		}
	}
    CalcBackwardVisitor<MAX_SUBBITS,&NoCryptoFA::InstructionMetadata::post,&NoCryptoFA::InstructionMetadata::post_own> cbv;
	for(llvm::Instruction::use_iterator it = ptr->use_begin(); it != ptr->use_end(); ++it) {
		if(Instruction* _it = dyn_cast<Instruction>(*it)) {
			NoCryptoFA::InstructionMetadata* usemd = NoCryptoFA::known[_it];
            cbv.md = md;
            cbv.usemd = usemd;
            cbv.visit(_it);
		}
	}

    if(!areVectorOfBitsetEqual<MAX_SUBBITS>(oldPost, md->post)) { changed = true; }
	if(changed) {
		for(llvm::Instruction::op_iterator it = ptr->op_begin(); it != ptr->op_end(); ++it) {
			if(Instruction* _it = dyn_cast<Instruction>(*it)) {
				toBeVisited.insert(_it);
			}
		}
	}
}

	void checkPost_masking(Instruction* ptr)
	{
	    NoCryptoFA::InstructionMetadata* md = NoCryptoFA::known[ptr];
	    if(!md->isAKeyOperation) {return;}
	    bool changed = false;
	    bool oldProt = md->hasToBeProtected_post;
	    if(!md->hasMetPlaintext) { md->hasToBeProtected_post = false;}
	    else{
		calcStatistics<MAX_SUBBITS,MAX_KEYBITS>(md->post_stats, md->post,md->post_keydep,1);
		NeedsMaskPostVisitor nmpv;
		nmpv.visit(ptr);
		if(oldProt != md->hasToBeProtected_post) { changed = true; }
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
