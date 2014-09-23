#ifndef ANALYSIS
#define ANALYSIS
#include "Analysis.h"
#endif

namespace llvm
{
namespace NoCryptoFA{
class ForwardAnalysis : public Analysis {

public:
ForwardAnalysis(): Analysis() {}

void calcAnalysis(){
	set<Instruction*> vulnerableTop;
	list<pair<int,Instruction*> > sortedList;
	sortedList.insert(sortedList.begin(),candidateVulnerablePointsPT.begin(),candidateVulnerablePointsPT.end());

	lookForMostVulnerableInstructionRepresentingTheEntireUserKey(sortedList,&vulnerableTop,&NoCryptoFA::InstructionMetadata::isVulnerableTopSubKey);

	vector<bitset<MAX_KEYBITS> > pre_subkeytokey = assignKeyOwn<MAX_SUBBITS>(vulnerableTop,&NoCryptoFA::InstructionMetadata::pre_own,&MSBEverSet,"vuln_top");

	runBatched(vulnerableTop, [this](Instruction * p,long batchn)->bool {calcPre(p);return false;});
	errs() << "Vuln=";
	errs () << vulnerableTop.size();
	errs() << "\nPRE-subkeys prop done\n";

	runBatched(keyStarts, [pre_subkeytokey,this](Instruction * p,long batchn)->bool {
		llvm::NoCryptoFA::InstructionMetadata*md = getMD(p);
		if(md->pre_keydep.size() > 0){return false;}
		md->pre_keydep.resize(md->pre.size());
		for(unsigned long i =0;i< md->pre.size();i++){
			bitset<MAX_KEYBITS> kb=0;
			for(unsigned long j =0;j< MAX_SUBBITS;j++){
				if(md->pre[i][j]){
				kb |= pre_subkeytokey[j];
				}
			}
			md->pre_keydep[i] = kb;
		}

		for(llvm::Instruction::use_iterator it = p->use_begin(); it != p->use_end(); ++it) {
			if(Instruction* _it = dyn_cast<Instruction>(*it)) {
			toBeVisited.insert(_it);
			}
		}
	return false;
	});

	runBatched(vulnerableTop, [this](Instruction * p,long batchn)->bool {checkPre_masking(p);return false;});

}

void calcPre(llvm::Instruction* ptr)
{
    NoCryptoFA::InstructionMetadata* md = NoCryptoFA::known[ptr];
    bool changed = false;
    vector<bitset<MAX_SUBBITS> > oldPre = md->pre;
    ClearMatrix<MAX_SUBBITS>(md->pre);
    CalcForwardVisitor<MAX_SUBBITS,&NoCryptoFA::InstructionMetadata::pre,&NoCryptoFA::InstructionMetadata::pre_own> cfv;
    cfv.visit(ptr);
    if(!areVectorOfBitsetEqual<MAX_SUBBITS>(oldPre, md->pre)) { changed = true; }
    if(changed || md->pre_own.any()) {
        if(!ptr->use_empty()) {
            for(llvm::Instruction::use_iterator it = ptr->use_begin(); it != ptr->use_end(); ++it) {
                if(Instruction* _it = dyn_cast<Instruction>(*it)) {
                    toBeVisited.insert(_it);
                }
            }
        }
    }
}

void checkPre_masking(llvm::Instruction* ptr)
{
    NoCryptoFA::InstructionMetadata* md = NoCryptoFA::known[ptr];
    bool changed = false;
    bool oldProtected = md->hasToBeProtected_pre;
    if(!md->hasMetPlaintext) { md->hasToBeProtected_pre = false;}
    else{
        calcStatistics<MAX_SUBBITS,MAX_KEYBITS>(md->pre_stats, md->pre,md->pre_keydep);
        NeedsMaskPreVisitor nmpv;
        nmpv.visit(ptr);
    }
    if(md->hasToBeProtected_pre != oldProtected){changed=true;}
        if(!ptr->use_empty()) {
            for(llvm::Instruction::use_iterator it = ptr->use_begin(); it != ptr->use_end(); ++it) {
                if(Instruction* _it = dyn_cast<Instruction>(*it)) {
                    toBeVisited.insert(_it);
                }
            }
        }
 }


};

}
}
