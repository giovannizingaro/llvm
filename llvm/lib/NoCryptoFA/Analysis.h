#pragma once
#include "llvm/IR/GlobalVariable.h"
#include "DeadBits.h"

static cl::opt<unsigned int>
SecurityMargin("nocryptofa-security-margin", cl::init(128), cl::ValueRequired,
               cl::desc("NoCryptoFA Security Margin (bits)"));

#define is_bit_set(what,num) ((what) & (1<<(num)))
#include "CalcPostVisitor.h"
#include "CalcPreVisitor.h"
#include "CalcBackwardFAVisitor.h"

#include "NeedsMaskPostVisitor.h"
#include "NeedsMaskPreVisitor.h"


using namespace llvm;

namespace std {
template<unsigned long N>
struct less<bitset<N> > {
bool operator()(const bitset<N> &B1, const bitset<N> &B2) const {
  for (unsigned i = 0; i != N; ++i)
    if (B1[i] != B2[i])
      return B1[i] < B2[i];

  return false;
}
};
}

namespace llvm
{
namespace NoCryptoFA{

		struct KeyStartInfo {
			public:
				llvm::Value* ptr;
				long index;
				KeyStartInfo(llvm::Value* _ptr, long _idx) {
					ptr = _ptr;
					index = _idx;
				}
				KeyStartInfo(llvm::Value* _ptr) {
					ptr = _ptr;
					index = -1;
				}
				KeyStartInfo() {
					index = -1;
					ptr = NULL;
				}
				friend bool operator== (const KeyStartInfo& a, const KeyStartInfo& b);
				friend bool operator< (const KeyStartInfo& a, const KeyStartInfo& b);
		};
		inline bool operator== (const NoCryptoFA::KeyStartInfo& a, const NoCryptoFA::KeyStartInfo& b)
		{
			return a.index == b.index && a.ptr == b.ptr;
		}
		inline bool operator< (const NoCryptoFA::KeyStartInfo& a, const NoCryptoFA::KeyStartInfo& b)
		{
			if(a.ptr != b.ptr) {
				return a.ptr < b.ptr;
			} else {
				return a.index < b.index;
			}
		}


class Analysis {

public:
	Analysis(){}

static void setAsTransformed(Function* ptr) {
	alreadyTransformed.insert(ptr);
}

static unsigned int getOperandSize(llvm::Instruction* ptr)
{
	return getOperandSize(ptr->getType());
}
static unsigned int getOperandSize(llvm::Type* t)
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

static unsigned int getMSBEverSet()
{
    return MSBEverSet;
}

static bool functionMarked(Function* ptr){

    return (markedfunctions.count(ptr) > 0);
}

static bool init(TaggedData& td,Function& Fun){
	keyLatestPos = 0;
	cipherOutPoints.clear();
	candidateVulnerablePointsPT.clear();
	candidateVulnerablePointsCT.clear();
	allKeyMaterial.clear();
	instr_bs.clear();
	keyStarts.clear();
	//set<Instruction*> keyStarts;
	//TaggedData& td = TaggedData::getAnalysis<TaggedData>();
	if(alreadyTransformed.find(&Fun) != alreadyTransformed.end()) {return false;}
	if(!td.functionMarked(&Fun)) {return false;}
	errs() << "Starting CalcDFG analysis\n";
	markedfunctions.insert(&Fun);
	for(llvm::Function::iterator FI = Fun.begin(),FE = Fun.end(); FI != FE; ++FI) {
		for(llvm::BasicBlock::iterator I = FI->begin(),E = FI->end(); I != E; ++I) {
			NoCryptoFA::known[I]->reset();
			if(NoCryptoFA::known[I]->isAKeyStart) {
				NoCryptoFA::known[I]->keydep_own = getOwnBitset(I);
				keyStarts.insert(I);
				//I->dump();
			}
		unsigned int size = getOperandSize(I);
		if(NoCryptoFA::known[I]->isAKeyOperation) {
			NoCryptoFA::known[I]->pre_keydep.resize(0);
			NoCryptoFA::known[I]->post_keydep.resize(0);
			NoCryptoFA::known[I]->keydep.resize(size);
			NoCryptoFA::known[I]->post.resize(size);
			NoCryptoFA::known[I]->pre.resize(size);
			NoCryptoFA::known[I]->out_hit.resize(size);

			for(unsigned int i = 0; i < size; ++i) {
			NoCryptoFA::known[I]->keydep[i] = bitset<MAX_KEYBITS>(0);
			NoCryptoFA::known[I]->pre[i] = bitset<MAX_SUBBITS>(0);
			NoCryptoFA::known[I]->post[i] = bitset<MAX_SUBBITS>(0);
			NoCryptoFA::known[I]->out_hit[i] = bitset<MAX_OUTBITS>(0);
			}
			calcDeadBits(I);
			}
		NoCryptoFA::known[I]->LinearDeps.resize(size);
		NoCryptoFA::known[I]->NonLinearDeps.resize(size);
		}
	}
	MSBEverSet=keyLatestPos;
	runBatched(keyStarts, [](Instruction * p,long batchn)->bool {searchCipherOutPoints(p); return false;});
	runBatched(cipherOutPoints, [](Instruction * p,long batchn)->bool {fillCiphertextHeight(p,batchn); return false;});
	runBatched(keyStarts, [](Instruction * p,long batchn)->bool {calcKeydep(p); return false;});
	return true;
}
//	virtual void calcAnalysis();

//protected:
	static std::set<Instruction*> cipherOutPoints;
	static std::multimap<int,Instruction*> candidateVulnerablePointsCT;
	static std::multimap<int,Instruction*> candidateVulnerablePointsPT;
	static std::set<Instruction*> allKeyMaterial;
	static unsigned int keyLatestPos;
        static unsigned int MSBEverSet;
	static set<Instruction*> keyStarts;
	static std::map<llvm::Instruction*, std::bitset<MAX_KEYBITS> > instr_bs;
	static std::set<Function*> alreadyTransformed;
	static std::set<Function*> markedfunctions;
	static std::map<NoCryptoFA::KeyStartInfo, std::bitset<MAX_KEYBITS> > GEPs;
	static std::set<Instruction*> toBeVisited;


//Metodi per l'analisi
	struct doubt{
	public:
	    bitset<MAX_KEYBITS> taken;
	    bitset<MAX_KEYBITS> leftover;
	    int valuesize;
	    doubt(bitset<MAX_KEYBITS> _taken,bitset<MAX_KEYBITS> _left,int n){
		taken=_taken; leftover=_left; valuesize=n;
	    }
	};

	template< int BITNUM>
	static void ClearMatrix(vector<bitset<BITNUM> >& vec)
	{
		for(unsigned int i = 0; i < vec.size(); i++) {
			vec[i].reset();
		}
	}

	template<int MAXBITS>
	void setDiagonal(vector<bitset<MAXBITS> >& data,bitset<MAXBITS> ownkey){
	    int datapos=0;
	    for(int pos=0;pos<MAXBITS;pos++){
		if(ownkey[pos]){
		    data[datapos][pos] = 1;
		    datapos++;
		    assert((size_t)datapos <= data.size());
		}
	    }
	}

static	void runBatched(set<Instruction*> initialSet, std::function<bool (Instruction*,long batchn)> func)
	{
		bool stopIterations = false;
		toBeVisited = initialSet;
	    long counter = 0;
		while((toBeVisited.size() > 0) && !stopIterations) {
			std::set<Instruction*> thisVisit = set<Instruction*>(toBeVisited);
			toBeVisited.clear();
		for(Instruction * p : thisVisit) {
		    if(func(p,counter)) {stopIterations = true;}
			}
		counter++;
		}
	}

static	void lookForMostVulnerableInstructionRepresentingTheEntireUserKey(list<pair<int,Instruction*> >& sorted,set<Instruction*>* most_vulnerable_instructions,bool NoCryptoFA::InstructionMetadata::* marker)
	{
	  bitset<MAX_KEYBITS> covered = 0; //Covered from the previous generation
	  bitset<MAX_KEYBITS> covering = 0; //Covered in this generation
	  bitset<MAX_KEYBITS> newbits,userkey_dep_full,mandatory = 0;
	  list<doubt> doubts = list<doubt>();
	  NoCryptoFA::InstructionMetadata* md;
	  int old_gen=-1;
	  for(auto it = sorted.begin(); it!= sorted.end();++it ){
		it->second->dump();
		if( it->first != old_gen){
		covered |= covering;
		covering=0;
		old_gen=it->first;
		if(covered.count() == keyLatestPos) {break;} 
		}
	     md = getMD(it->second);
	     userkey_dep_full = massiveOR(md->keydep);
	     newbits = userkey_dep_full & ~covered;
	     mandatory=checkForOptimizations(covered,covering,userkey_dep_full,md->keydep.size(),doubts);
	     newbits |= mandatory;
	     if(newbits.count() > 0){
	      newbits=newbits &~ covering;
	      assert((newbits & mandatory) == mandatory);
	      newbits |= mandatory;
	      newbits=limitTakenBits(md->keydep.size(),newbits,mandatory,doubts);
	      covering |= newbits;
	      most_vulnerable_instructions->insert(it->second);
	      md->*marker=true;

	    }
	  }

	  doubts.clear();
	}

static llvm::NoCryptoFA::InstructionMetadata* getMD(llvm::Instruction* ptr)
{
		return NoCryptoFA::known[ptr];
}

static bool compare_line_number(Instruction* a,Instruction* b){
    return a->getDebugLoc().getLine() < b->getDebugLoc().getLine();
}

template <int SIZE>
static vector<bitset<MAX_KEYBITS> > assignKeyOwn(set<Instruction*> instructions,bitset<SIZE> NoCryptoFA::InstructionMetadata::*OWN,unsigned int* msb,std::string dbginfo){
    unsigned int latestPos=0;
    vector<bitset<MAX_KEYBITS> > subkeytokey;
    subkeytokey.resize(SIZE);
    vector<Instruction*> ptrs(instructions.size());
    std::copy(instructions.begin(),instructions.end(),ptrs.begin());
    std::stable_sort(ptrs.begin(),ptrs.end(),compare_line_number);

    for(Instruction* p: ptrs){
        NoCryptoFA::InstructionMetadata *md = getMD(p);
        md->*OWN=getOutBitset<SIZE>(p,latestPos,dbginfo);
        int setbit = 0;
        for(int i = 0;i < SIZE; i++){
            if((md->*OWN).test(i)){
                subkeytokey[i] = md->keydep[setbit++];
            }
        }

    }
    *msb=std::max(latestPos,*msb);
    return subkeytokey;
}

static bitset<MAX_KEYBITS> massiveOR(std::vector<bitset<MAX_KEYBITS> >& input){
	bitset<MAX_KEYBITS> max(0);
	for(auto &v : input ){
		max |= v;
	}
	return max;
}

static bitset<MAX_KEYBITS> checkForOptimizations(bitset<MAX_KEYBITS>& covered,bitset<MAX_KEYBITS>& covering,bitset<MAX_KEYBITS>& userkey_dep_full,unsigned int num,list<doubt>& doubts){
        bitset<MAX_KEYBITS> mandatory = 0;
        bitset<MAX_KEYBITS> new_previously_covered = 0;
        bitset<MAX_KEYBITS> new_mandatory= 0;
        int howmany;
        doubts.remove_if([covered,covering](doubt& val){
            return ((val.leftover & covered & covering).count() == 0); //Housekeeping
        });


        for(auto u = doubts.begin(); u != doubts.end(); ++u){
              if((u->leftover &~ userkey_dep_full).count()>0){   //Bits to be covered with high priority, which I can't take care of.
                 if((u->taken & userkey_dep_full).count() > 0) { //Bits already covered, which I can re-cover.
                  //sposto un bit da quell'insieme all'altro
                  howmany = num-mandatory.count();
                  new_mandatory =getFirstNSetBit(u->taken & userkey_dep_full,howmany);
                  mandatory |= new_mandatory;
                  u->taken &= ~new_mandatory;
                  covered &= ~new_mandatory;
                  howmany = new_mandatory.count();
                  new_previously_covered = getFirstNSetBit(u->leftover & ~userkey_dep_full,howmany);
                  u->taken |= new_previously_covered;
                  u->leftover &= ~new_previously_covered;
                  covered |= new_previously_covered;
                  if(mandatory.count() == num){break;}
                }
            }

        }
      return mandatory;
    }
static bitset<MAX_KEYBITS> limitTakenBits(unsigned int max,bitset<MAX_KEYBITS>& newbits,bitset<MAX_KEYBITS>& mandatory, list<doubt>& doubts ){
      bitset<MAX_KEYBITS> tobereturned=newbits;
      bitset<MAX_KEYBITS> newbits_mask = 0x1;
      assert(mandatory.count() <= max);
      if(newbits.count() <= max) { return newbits;}
      while(tobereturned.count() > max){
        tobereturned &= ~newbits_mask;
        tobereturned |= mandatory;
        newbits_mask <<= 1; //If it's not enough, try clearing the next one.
      } // End of while(newbits.countOnes() > max)
      doubts.push_front(doubt(tobereturned,newbits &~ tobereturned, max));
      return tobereturned;
}

template<int SIZE>
static bitset<SIZE> getOutBitset(llvm::Instruction* ptr,unsigned int& latestPos,std::string dbginfo)
{
	Value* op = ptr;
    unsigned int outQty = getOperandSize(op->getType());
    if(latestPos + outQty > SIZE) {
        errs() << "Something wrong with CalcDFG: " << latestPos << " + " << outQty << " > " << SIZE << " for instruction " << *ptr <<"\n";
        return bitset<SIZE>(0);
	}
    //  cerr << "latestPos " << latestPos << " outQty:" << outQty << endl;
    bitset<SIZE> mybs;
	mybs.reset();
    cerr << dbginfo << " bits " << latestPos << "-" << latestPos+outQty << " assigned at line " << ptr->getDebugLoc().getLine() << endl;
    for(unsigned int i = latestPos; i < (latestPos + outQty); i++) {
		mybs[i] = 1;
	}
    latestPos += outQty;

    //cerr << " new latestPos " << latestPos << " riga " << ptr->getDebugLoc().getLine() << endl;
	return mybs;
}


static bitset<MAX_KEYBITS> getFirstNSetBit(bitset<MAX_KEYBITS> input,unsigned int howmany){
      bitset<MAX_KEYBITS> newbits_mask = 0x1;
      while(input.count() > howmany){
          input &= ~newbits_mask;
          newbits_mask <<= 1; //If it's not enough, try clearing the next one.
      } // End of while(newbits.countOnes() >howmany)
    return input;
}


template< int BITNUM>
static bool areVectorOfBitsetEqual(vector<bitset<BITNUM> >& vec1, vector<bitset<BITNUM> >& vec2)
{
	if(vec1.size() != vec2.size()) { return false; }
	return (memcmp(vec1.data(), vec2.data(), sizeof(bitset<BITNUM>) * vec1.size()) == 0);
}

template<int NUMBITS,int NUMBITS2>
static void calcStatistics(llvm::NoCryptoFA::StatisticInfo& stat, vector<bitset<NUMBITS> >& vect,vector<bitset<NUMBITS2> >& vect2, int base)
{
    int avgcnt = 0;
    int avgnzcnt = 0;
    int cnt = 0;
    stat.min = MAX_PROTECTION;
    stat.min_nonzero = MAX_PROTECTION;
    //std::cerr << "vect.size() == " << vect.size() << " vect2.size() == " << vect2.size() << std::endl;
    assert(vect.size() == vect2.size());
	if(vect.size()%base != 0)
                errs () << "Errore";

    for(unsigned long i = 0;i<vect.size();i+=base) {
	bitset<NUMBITS> tmp1;
	bitset<NUMBITS2> tmp2;
	for(int j=0;j<base;j++){
		tmp1|=vect[i+j];
		tmp2|=vect2[i+j];
	}
	cnt = std::min(tmp1.count(),tmp2.count());
	//cnt = std::min(vect[i].count(),vect2[i].count());
        avgcnt++;
        stat.max = std::max(stat.max,cnt);
        stat.min = std::min(stat.min,cnt);
        if(cnt > 0) {
            stat.avg_nonzero += cnt;
            stat.avg += cnt;
            avgnzcnt++;
            stat.min_nonzero = std::min(stat.min_nonzero,cnt);
        }
	//stat.min_nonzero = 1;
    }
    if(stat.min == 0 && stat.min_nonzero==MAX_PROTECTION) {stat.min_nonzero=0;}
    if(avgcnt > 0) { stat.avg = stat.avg / avgcnt; }
    if(avgnzcnt > 0) { stat.avg_nonzero = stat.avg_nonzero / avgnzcnt; }
}
/*
template<int NUMBITS,int NUMBITS2>
static void calcStatistics(llvm::NoCryptoFA::StatisticInfo& stat, vector<bitset<NUMBITS> >& vect,vector<bitset<NUMBITS2> >& vect2)
{
    int avgcnt = 0;
    int avgnzcnt = 0;
    int cnt = 0;
    stat.min = MAX_PROTECTION;
    stat.min_nonzero = MAX_PROTECTION;
    //std::cerr << "vect.size() == " << vect.size() << " vect2.size() == " << vect2.size() << std::endl;
    assert(vect.size() == vect2.size());
    for(unsigned long i = 0;i<vect.size();i++) {
        cnt = std::min(vect[i].count(),vect2[i].count());
        avgcnt++;
        stat.max = std::max(stat.max,cnt);
        stat.min = std::min(stat.min,cnt);
        if(cnt > 0) {
            stat.avg_nonzero += cnt;
            stat.avg += cnt;
            avgnzcnt++;
            stat.min_nonzero = std::min(stat.min_nonzero,cnt);
        }
    }
    if(stat.min == 0 && stat.min_nonzero==MAX_PROTECTION) {stat.min_nonzero=0;}
    if(avgcnt > 0) { stat.avg = stat.avg / avgcnt; }
    if(avgnzcnt > 0) { stat.avg_nonzero = stat.avg_nonzero / avgnzcnt; }
}
*/

static void searchCipherOutPoints(llvm::Instruction* ptr){
    NoCryptoFA::InstructionMetadata* md = NoCryptoFA::known[ptr];
    if(md->hasMetPlaintext && md->isAKeyOperation && ptr->use_empty()) {
        cipherOutPoints.insert(ptr);
    }
            for(llvm::Instruction::use_iterator it = ptr->use_begin(); it != ptr->use_end(); ++it) {
                if(Instruction* _it = dyn_cast<Instruction>(*it)) {
                    toBeVisited.insert(_it);
                }
            }

}
static void calcKeydep(llvm::Instruction* ptr)
{
	NoCryptoFA::InstructionMetadata* md = NoCryptoFA::known[ptr];
	bool changed = false;
    vector<bitset<MAX_KEYBITS> > oldKeydep = md->keydep;
    ClearMatrix<MAX_KEYBITS>(md->keydep);
    CalcForwardVisitor<MAX_KEYBITS,&NoCryptoFA::InstructionMetadata::keydep,&NoCryptoFA::InstructionMetadata::keydep_own> cfv;
    cfv.visit(ptr);
    if(!areVectorOfBitsetEqual<MAX_KEYBITS>(oldKeydep, md->keydep)) { changed = true; }
    if(changed || md->keydep_own.any()) {
        calcStatistics<MAX_KEYBITS,MAX_KEYBITS>(md->keydep_stats, md->keydep,md->keydep,1);
		if(!ptr->use_empty()) {
			for(llvm::Instruction::use_iterator it = ptr->use_begin(); it != ptr->use_end(); ++it) {
				if(Instruction* _it = dyn_cast<Instruction>(*it)) {
					toBeVisited.insert(_it);
                    if((NoCryptoFA::known[_it]->hasMetPlaintext && NoCryptoFA::known[_it]->isAKeyOperation)) {
                        if(!md->hasMetPlaintext){
                            //Insert the edge of the graph into the multimap.
                            //Each element can appear multiple times with different heights
                            //but only one time for each height
                            { // for plaintext
                                const int h = NoCryptoFA::known[_it]->PlaintextHeight;
                                auto equalheight = candidateVulnerablePointsPT.equal_range(h);
                                if(std::find(equalheight.first,equalheight.second,std::pair<const int,Instruction*>(h,ptr)) == equalheight.second){
                                        //It's new, let's insert it.
                                    candidateVulnerablePointsPT.insert(std::make_pair(h,ptr));
                                    md->isSubKey=true;
                                }
                            }
                            { // for ciphertext
                                const int h = NoCryptoFA::known[_it]->CiphertextHeight;
                                auto equalheight = candidateVulnerablePointsCT.equal_range(h);
                                if(std::find(equalheight.first,equalheight.second,std::pair<const int,Instruction*>(h,ptr)) == equalheight.second){
                                        //It's new, let's insert it.
                                    candidateVulnerablePointsCT.insert(std::make_pair(h,ptr));
                                    md->isSubKey=true;
                                }
                            }
                            { //for faults
                                allKeyMaterial.insert(ptr);
                            }
                        }
					}
				}
			}
		}
	}
}

static bitset<MAX_KEYBITS> getOwnBitset(llvm::Instruction* ptr)
{
	raw_fd_ostream rerr(2, false);
	if(instr_bs.find(ptr) != instr_bs.end()) {
		return instr_bs[ptr];
	}
	Type* t = NULL;
	if(isa<llvm::GetElementPtrInst>(ptr)) {
		GetElementPtrInst* gep = cast<GetElementPtrInst>(ptr);
		if(!gep->hasAllConstantIndices()) {cerr << "GetOwnBitset on a non-constant GetElementPtr. Dow!" << endl;}
		if(gep->getNumIndices() != 1) {cerr << "GetOwnBitset on a GetElementPtr with more than 1 index. Dow!" << endl; }
		Value* idx = gep->getOperand(1);
		if(isa<ConstantInt>(idx)) {
			ConstantInt* ci = cast<ConstantInt>(idx);
			NoCryptoFA::KeyStartInfo* me = new NoCryptoFA::KeyStartInfo(gep->getPointerOperand(), ci->getZExtValue());
			if(GEPs.find(*me) != GEPs.end()) {
				return GEPs[*me];
			} else {
				int keyQty = getOperandSize(ptr);
				bitset<MAX_KEYBITS> mybs;
				mybs.reset();
                for(unsigned int i = keyLatestPos; i < (keyLatestPos + keyQty); i++) {
					mybs[i] = 1;
				}
				keyLatestPos += keyQty;
				// cerr << "kq: "<<keyQty<<  " lp " << latestPos << "--"<< mybs.to_string() << endl;
				instr_bs[ptr] = mybs;
				return mybs;
			}
		} else {
			cerr << "not even the first is a costant index. Dow!" << endl;
		}
	} else {
		t = ptr->getType();
	}
	int keyQty = getOperandSize(t);
	bitset<MAX_KEYBITS> mybs;
	mybs.reset();
    for(unsigned int i = keyLatestPos; i < (keyLatestPos + keyQty); i++) {
		mybs[i] = 1;
	}
	keyLatestPos += keyQty;
    //cerr << "nuovo kLP " << keyLatestPos << endl;
	//  cerr << "kq: "<<keyQty<<  " lp " << latestPos << "--"<< mybs.to_string() << endl;
	instr_bs[ptr] = mybs;
	return mybs;
}

static void fillCiphertextHeight(Instruction *ptr, int batchn){
    llvm::NoCryptoFA::InstructionMetadata* md = getMD(ptr);
    if(batchn < md->CiphertextHeight){
        md->CiphertextHeight = batchn;
    }
    for(llvm::Instruction::op_iterator it = ptr->op_begin(); it != ptr->op_end(); ++it) {
        if(Instruction* _it = dyn_cast<Instruction>(*it)) {
            toBeVisited.insert(_it);
        }
    }
}




}; // fine classe

	std::set<Instruction*> Analysis::cipherOutPoints;
	std::multimap<int,Instruction*> Analysis::candidateVulnerablePointsCT;
	std::multimap<int,Instruction*> Analysis::candidateVulnerablePointsPT;
	std::set<Instruction*> Analysis::allKeyMaterial;
	unsigned int Analysis::keyLatestPos;
        unsigned int Analysis::MSBEverSet;
	set<Instruction*> Analysis::keyStarts;
	std::map<llvm::Instruction*, std::bitset<MAX_KEYBITS> > Analysis::instr_bs;
	std::set<Function*> Analysis::alreadyTransformed;
	std::set<Function*> Analysis::markedfunctions;
	std::map<NoCryptoFA::KeyStartInfo, std::bitset<MAX_KEYBITS> > Analysis::GEPs;
	std::set<Instruction*> Analysis::toBeVisited;

}
}
