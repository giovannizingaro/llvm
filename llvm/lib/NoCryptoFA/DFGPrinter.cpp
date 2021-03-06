#include <iostream>
#include <map>
#include <iomanip>

#include <list>
#include <fstream>
#include <sstream>
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/Analysis/DOTGraphTraitsPass.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/Module.h"
#include "llvm/Analysis/Dominators.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Metadata.h"
#include <llvm/Pass.h>
#include "llvm/IR/Function.h"
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/GraphWriter.h>
#include <llvm/ADT/GraphTraits.h>
#include <llvm/IR/IntrinsicInst.h>
#include <llvm/NoCryptoFA/All.h>
#include <llvm/Support/CommandLine.h>
#include <semaphore.h>

#define PRINT(rounds,type,max)						\
{									\
                string output_str = string();				\
                output_str.reserve(10*1024*1024);			\
                llvm::raw_string_ostream output(output_str);		\
                output << "x y z";					\
                for(int y = 0; y<rounds.size();y++)			\
                        for(int x =0; x<max; x++){			\
				if(x < rounds.at(y).type.size())	\
                                	output << "\n" << x << " " << y << " " <<  rounds.at(y).type.at(x);\
				else					\
					output << "\n" << x << " " << y << " " << 20000;		\
			}						\
                outFile("../" #type ".dat", output.str());		\
}

using namespace llvm;
using namespace std;
//aggiunto
static cl::opt<string>
RigheCifrario("righe-cifrario", cl::init(""), cl::ValueRequired,
               cl::desc("Passare il file righe cifrario"));
//fine aggiunto
static cl::opt<bool>
PrintDot("print-dot", cl::init(false), cl::ValueOptional,
               cl::desc("Print DOT file on stdout"));
static cl::opt<bool>
PrintCSV("print-csv", cl::init(false), cl::ValueOptional,
               cl::desc("Print CSV file on out.dir/function_name.csv"));
static cl::opt<bool>
PrintHTML("print-html", cl::init(false), cl::ValueOptional,
               cl::desc("Print an html file for each node of dot file in outdir/nodename.html"));

MyNodeType* MyNodeType::rootnode = NULL;
namespace llvm
{

	template<> struct GraphTraits<MyNodeType*> {
		typedef MyNodeType NodeType;

		typedef std::set<MyNodeType*>::iterator nodes_iterator;
		typedef std::set<MyNodeType*>::iterator ChildIteratorType;
		static NodeType* getEntryNode(MyNodeType* n) { return n; }
		static inline nodes_iterator nodes_begin(NodeType* N) {
			return N->subnodes.begin();
		}
		static inline nodes_iterator nodes_end(NodeType* N) {
			return N->subnodes.end();
		}
		static inline ChildIteratorType child_begin(NodeType* N) { return N->children.begin();}
		static inline ChildIteratorType child_end(NodeType* N) { return N->children.end();}
	};


	template<>
	struct DOTGraphTraits<MyNodeType*> : public DefaultDOTGraphTraits {

		DOTGraphTraits (bool isSimple = false)
			: DefaultDOTGraphTraits(isSimple) {}

		std::string getNodeLabel(MyNodeType* Node, MyNodeType* Graph) {
			return Node->name;
		}

		std::string getNodeAttributes(MyNodeType* Node,
		                              const MyNodeType* Graph) {
			if(Node->md) {
				switch(Node->md->origin) { //Sta diventando piu spaghettoso di quanto sia giusto. Refactor?
					case NoCryptoFA::InstructionMetadata::ORIGINAL_PROGRAM:
                        if(Node->md->isVulnerableTopSubKey || Node->md->isVulnerableBottomSubKey) {
							return "style=filled,color=\"#00ff00\"";
                        }else if(Node->md->isSubKey) {
                            return "style=filled,color=\"#ffff80\"";
                        }
						if(Node->hasToBeProtected) {
							return "style=filled,color=\"#f458f4\"";
						} else if(Node->md->isAKeyOperation) {
							return "style=filled,color=\"#58faf4\"";
						}
						break;
					case NoCryptoFA::InstructionMetadata::CREATE_MASK:
						return "style=filled,color=\"#181af4\"";
						break;
					case NoCryptoFA::InstructionMetadata::REMOVE_MASK:
						return "style=filled,color=\"#ff1a04\"";
						break;
					default:
						return "style=filled,color=\"#18fa04\"";
						break;
				}
			}
			return "style=filled,color=\"#e0e0e0\"";
		}
		template<typename EdgeIter>
		std::string getEdgeAttributes(const MyNodeType* Node, EdgeIter EI,
		                              const MyNodeType* Graph) {
			if(Node->md) {
				if(Node->md->hasToBeProtected_pre || Node->md->hasToBeProtected_post) {
					return "color=\"#f458f4\"";
				} else if(Node->md->isAKeyOperation) {
					return "color=\"#58faf4\"";
				}
			}
			return "color=\"#e0e0e0\"";
		}
	};



	char DFGPrinter::ID = 22;

}
void MyNodeType::addChildren(MyNodeType* nuovo)
{
	children.insert(nuovo);
	MyNodeType::rootnode->subnodes.insert(nuovo);
}

void DFGPrinter::print(raw_ostream& OS, const Module* ) const
{
	GraphWriter<MyNodeType*> gw(OS, rootptr, true);
	gw.writeGraph("");
}
void replaceAll(std::string& str, const std::string& from, const std::string& to)
{
	if(from.empty())
	{ return; }
	size_t start_pos = 0;
	while((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}
template <int SIZE>
string printvec_small(std::vector<bitset<SIZE> >& v)
{
	stringstream ss("");
	size_t max = 0;
for(bitset<SIZE> b : v) {
		max = std::max(max, b.count());
	}
	ss << "max:" << max << "bit";
	return ss.str();
}
template <int SIZE>
string printbs_syntethic(bitset<SIZE>& b)
{
	stringstream ss("");
	int value = 0;
	for(int i = 0; i < SIZE; i++) {
		if(b[i]) {
			value++;
		}
		if((i % 32) == 0) {
			ss << hex << setw(8) << internal << setfill('0') << value;
			value = 0;
		}
		value <<= 1;
	}
	return ss.str();
}
template <int SIZE>
string print_syntethic(std::vector<bitset<SIZE> >& v)
{
	stringstream ss("");
for(bitset<SIZE> b : v) {
		ss << printbs_syntethic<SIZE>(b);
	}
	return ss.str();
}
static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";
std::string base64_encode(char const* bytes_to_encode, unsigned int in_len) {
  std::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while((i++ < 3))
      ret += '=';

  }

  return ret;

}
/*
template <unsigned int SIZE>
string printvec_large(std::vector<bitset<SIZE> >& v,unsigned int upto)
{
	stringstream ss("");
	ss << "<div class=\"matrice\">";
for(bitset<SIZE> b : v) {
		ss << "<div class=\"row\">";
        for(unsigned int s = 0; s < std::min(upto,SIZE); ++s) {
			if(b[s]) {
				ss << "<b></b>";
			} else {
				ss << "<i></i>";
			}
		}
		ss << "</div>";
	}
	ss << "</div>";
	return ss.str();
}
*/

template <unsigned int SIZE>
string printvec_large(std::vector<bitset<SIZE> >& v,unsigned int upto,std::string name)
{
    int rows = v.size();
    int cols = 8*ceil(std::min(upto,SIZE)/8.0f);
    std::string buffer;
    buffer = "";
    buffer.reserve(rows*cols/8);
    stringstream ss("");
    ss << "<canvas height=\"" << rows << "\" width=\"" << cols << "\" id=\"" << name << "\"></canvas>";

    char tobewritten = 0;
    int  written_bit = 0;
    for(int cur_row = 0; cur_row < rows; cur_row++){
        for(int s = 0; s < cols; ++s) {
            written_bit++;
            tobewritten <<= 1;
            if(v[cur_row][s]) {
                tobewritten |= 1;
            }
            if(written_bit == 8){
                written_bit = 0;
                buffer.push_back(tobewritten);
                tobewritten = 0;
            }
        }
        assert(written_bit==0);
    }
        ss << "<script type=\"text/javascript\">writedata(\"" << name <<"\"," << rows << "," << cols <<" , \"" << base64_encode(buffer.data(),buffer.size()) << "\")</script>";

    return ss.str();
}

template <int SIZE>
string printbs_small(bitset<SIZE>& bs)
{
	stringstream ss("");
	ss << bs.count();
	return ss.str();
}
template <int SIZE>
string printbs_large(bitset<SIZE>& bs)
{
	string in = bs.to_string();
	/*in = string ( in.rbegin(), in.rend() );
	in = string("-").append(in);
	string::size_type last= in.find_last_not_of('0');
	if(last != in.npos){
	    in=in.erase(1+last);
	}*/
	replaceAll(in, "00000000", "a");
	replaceAll(in, "11111111", "A");
	/*replaceAll(in,"aaaaaaaa","b");
	replaceAll(in,"AAAAAAAA","B");
	replaceAll(in,"bbbbbbbb","c");
	replaceAll(in,"BBBBBBBB","C");*/
	return in.append("-");
}

void outFile(std::string nodename, std::string contenuto)
{
	string fname("out.dir/");
	ofstream out(fname.append(nodename));
	out << contenuto;
}
template <class AN>
AN& DFGPrinter::my_getAnalysis(Function* f){
    static std::map<Function*,AN&> cache;
    auto it = cache.find(f);
    if(it == cache.end()){
        AN& newone = getAnalysis<AN>(*f);
        cache.insert(std::pair<Function*,AN&>(f,newone));
        return newone;
    }
    else{
        return it->second;
    }
}


void DFGPrinter::doDAT(Module& M){
	//aggiunto
	typedef struct Round{
        int begin;
        int end;
        vector<int> forward;
	vector<int> backward;
	vector<int> prot_dpa;
	vector<int> nonlinbit;
	vector<int> nonlinbyte;
        } Round;

	vector<Round> rounds;
        string name = (RigheCifrario.getValue());
	std::ifstream infile(name);
	int a,b;
	Round round;
	while (infile >> a >> b)
	{
	round.begin = a;
	round.end = b;
	rounds.push_back(round);
    	//errs () << "IN:" << a << "FIN:" << b;
	}
	//errs () << "Sono stati trovati " << rounds.size() << ".";
//Fine

    errs() << "Starting DAT output\n";
    for(llvm::Module::iterator F = M.begin(), ME = M.end(); F != ME; ++F) {
        for(llvm::Function::iterator BB = F->begin(),
            FE = F->end();
            BB != FE;
            ++BB) {
            CalcDFG& cd = my_getAnalysis<CalcDFG>(F);

            if(!cd.functionMarked(&(*F))) { continue; }
            for( llvm::BasicBlock::iterator i = BB->begin(); i != BB->end(); i++) {
                if(isa<llvm::DbgInfoIntrinsic>(i)) {continue;}
                llvm::NoCryptoFA::InstructionMetadata* md = cd.getMD(i);
		//aggiunto
		for(std::vector<Round>::iterator r = rounds.begin(); r!=rounds.end();++r){
		if(i->getDebugLoc().getLine()>=r->begin && i->getDebugLoc().getLine()<=r->end){
		//Forward
		if(md->pre_stats.min_nonzero == 999999 )
			r->forward.push_back(0);
		else
			r->forward.push_back(md->pre_stats.min_nonzero);
                //Backward
                if(md->post_stats.min_nonzero == 999999 )
                        r->backward.push_back(0);
                else
                        r->backward.push_back(md->post_stats.min_nonzero);
		//Protdpa
                int prot = std::min(md->pre_stats.min_nonzero, md->post_stats.min_nonzero) ;
		if(prot == 999999 )
                        r->prot_dpa.push_back(0);
                else
                        r->prot_dpa.push_back(prot);
		//Nonlin byte
                if(md->NonLinStatsByte.min_nonzero == 999999 )
                        r->nonlinbit.push_back(0);
                else
                        r->nonlinbit.push_back(md->NonLinStats.min_nonzero);
                if(md->NonLinStats.min_nonzero == 999999 )
                        r->nonlinbyte.push_back(0);
                else
                        r->nonlinbyte.push_back(md->NonLinStatsByte.min_nonzero);

                }//fine if

		}//fine for

            }

		int max=0;
		for(std::vector<Round>::iterator r=rounds.begin(); r!=rounds.end(); ++r){
		if(r->forward.size()>max)
			max = r->forward.size();
		}
		//errs () << "Il massimo è:" << max;

		PRINT(rounds,forward,max);
		PRINT(rounds,backward,max);
		PRINT(rounds,prot_dpa,max);
		PRINT(rounds,nonlinbit,max);
		PRINT(rounds,nonlinbyte,max);
        }
    }

}

void DFGPrinter::doCSV(Module& M){
	
    errs() << "Starting CSV output\n";
    for(llvm::Module::iterator F = M.begin(), ME = M.end(); F != ME; ++F) {
        for(llvm::Function::iterator BB = F->begin(),
            FE = F->end();
            BB != FE;
            ++BB) {
            CalcDFG& cd = my_getAnalysis<CalcDFG>(F);
            string instr_dump_str = string();
            instr_dump_str.reserve(200*1024*1024); //200M, yes, I hate having to re-alloc something. Even if it is transparent to the developer. I need to use those 16GB of RAM.

            llvm::raw_string_ostream instr_dump(instr_dump_str);
            instr_dump << "Pre_Max;Pre_Min;Pre_MinNZ;Pre_Avg;Pre_AvgNZ;";
            instr_dump << "Post_Max;Post_Min;Post_MinNZ;Post_Avg;Post_AvgNZ;";
            instr_dump << "Min_MinNZ;Plaintext;PTHeight;CTHeight;ToBeProtected_pre;ToBeProtected_post;ToBeProtected;SourceLine;SourceColumn;";
            // parte per output dettagliato
            instr_dump << "IsAKeyOp;IsAKeyStart;PreKeyStart;SubKey;PostKeyStart;Sbox;post_FirstToMeetKey;HasBeenMasked;Origin;ValueSize;keydep_own.count;";
            instr_dump << "KD_Max;KD_Min;KD_MinNZ;KD_Avg;KD_AvgNZ;FK_min;FOH_of_fk_min;BYTE_FK_min;BYTE_FOH_of_fk_min;WORD_FK_min;WORD_FOH_of_fk_min";
            instr_dump << "pre;pre_own;post;post_own;";
            // fine parte per output dettagliato
            instr_dump << "\"Full instruction\"\n";
            if(!cd.functionMarked(&(*F))) { continue; }
            for( llvm::BasicBlock::iterator i = BB->begin(); i != BB->end(); i++) {
                if(isa<llvm::DbgInfoIntrinsic>(i)) {continue;}
                llvm::NoCryptoFA::InstructionMetadata* md = cd.getMD(i);
		//Output Forward
                instr_dump << md->pre_stats.max << ";";
                instr_dump << md->pre_stats.min << ";";
                instr_dump << md->pre_stats.min_nonzero << ";";
                instr_dump << md->pre_stats.avg << ";";
                instr_dump << md->pre_stats.avg_nonzero << ";";
		//Output Backward
                instr_dump << md->post_stats.max << ";";
                instr_dump << md->post_stats.min << ";";
                instr_dump << md->post_stats.min_nonzero << ";";
                instr_dump << md->post_stats.avg << ";";
                instr_dump << md->post_stats.avg_nonzero << ";";
		//Output Prot_DPA
                instr_dump << std::min(md->pre_stats.min_nonzero, md->post_stats.min_nonzero) << ";";

                instr_dump << md->hasMetPlaintext << ";";
                instr_dump << md->PlaintextHeight << ";";
                instr_dump << md->CiphertextHeight << ";";
                instr_dump << md->hasToBeProtected_pre << ";";
                instr_dump << md->hasToBeProtected_post << ";";
                instr_dump << (md->hasToBeProtected_pre | md->hasToBeProtected_post) << ";";
                if(i->getDebugLoc().isUnknown()) {
                    instr_dump << "UNKNOWN;UNKNOWN;";
                } else {
                    instr_dump << i->getDebugLoc().getLine() << ";";
                    instr_dump << i->getDebugLoc().getCol() << ";";
                }
                // parte per output dettagliato
                instr_dump << md->isAKeyOperation << ";";
                instr_dump << md->isAKeyStart << ";";
                instr_dump << md->isVulnerableTopSubKey << ";";
                instr_dump << md->isSubKey << ";";
                instr_dump << md->isVulnerableBottomSubKey << ";";
                instr_dump << md->isSbox << ";";
                instr_dump << md->post_FirstToMeetKey << ";";
                instr_dump << md->hasBeenMasked << ";";
                instr_dump << md->origin << ";";
                instr_dump << md->pre.size() << ";";
                instr_dump << md->keydep_own.count() << ";";
                instr_dump << md->keydep_stats.max << ";";
                instr_dump << md->keydep_stats.min << ";";
                instr_dump << md->keydep_stats.min_nonzero << ";";
                instr_dump << md->keydep_stats.avg << ";";
                instr_dump << md->keydep_stats.avg_nonzero << ";";
		instr_dump << 0 << ";";
		instr_dump << 0 << ";";
		instr_dump << 0 << ";";
		instr_dump << 0 << ";";
		instr_dump << 0 << ";";
		instr_dump << 0 << ";";

                /* Keep the code here, it might get useful in a far future... actually, it's not worth the time and filesize increase.
                instr_dump << print_syntethic<MAX_SUBBITS>(md->pre) << ";";
                instr_dump << printbs_syntethic<MAX_SUBBITS>(md->pre_own) << ";";
                instr_dump << print_syntethic<MAX_SUBBITS>(md->post) << ";";
                instr_dump << printbs_syntethic<MAX_SUBBITS>(md->post_own) << ";";*/

				if (i->getName() == "conv9216") {
					errs() << "Il porco grugnisce!!!\n";
				}


                instr_dump << md->NonLinStats.min_nonzero << ";";
                instr_dump << md->NonLinStats.max << ";";
                instr_dump << md->NonLinStats.min << ";";

                for (unsigned x : md->EquivocationCount)
                  instr_dump << x << " ";
                instr_dump << ";";
                // fine parte per output dettagliato
                instr_dump << "\"" << md->getAsString() << "\"\n";
            }
            outFile(F->getName().str().append(".csv"), instr_dump.str());
        }
    }

}


sem_t html_sem;
bool doHTML_instruction(Instruction* i, CalcDFG* cd){
    sem_wait(&html_sem);
    std::string outp;
    outp.reserve(200*1024); //200k. Smaller :)
    llvm::raw_string_ostream os (outp);
    std::stringstream boxcont("");
    std::stringstream fname("");
    boxcont << "<html><head><LINK REL=StyleSheet HREF=\"../node.css\" TYPE=\"text/css\"/><script type=\"text/javascript\" src=\"../node.js\"></script><script type=\"text/javascript\" src=\"https://www.google.com/jsapi\"></script></head><body><pre>";
    llvm::NoCryptoFA::InstructionMetadata* md = cd->getMD(i);
    //md->unpack();
    os << "I:<span>" << md->getAsString() << "</span>\n";
    if(md->isAKeyOperation) {
        if(md->isAKeyStart) {
            os << "KeyStart" << "\n";
        }
        os << "<Own:" << printbs_small<MAX_SUBBITS>(md->pre_own) << ",Pre:" << printvec_small<MAX_SUBBITS>(md->pre) << ",Post_Own:" << printbs_small<MAX_SUBBITS>(md->post_own) << ",Post:" << printvec_small<MAX_SUBBITS>(md->post) << ">" << "\n";
    }
    boxcont << os.str() << "\n";
    if(md->isSubKey)  boxcont << "SubKey\n";
    if(md->isVulnerableTopSubKey)  boxcont << "VulnTop\n";
    if(md->isVulnerableBottomSubKey)  boxcont << "VulnBottom\n";

    if(md->post_FirstToMeetKey) {
        boxcont << "Primo ad incontrare la chiave backwards\n";
    }
    if(md->hasMetPlaintext) {
        boxcont << "Ha incontrato il plaintext (" << md->PlaintextHeight << ")\n";
        boxcont << "Influenzer&agrave; ciphertext (" << md->CiphertextHeight << ")\n";
    } else {
        boxcont << "Non ha incontrato il plaintext\n";
    }
    switch(md->origin) {
        case NoCryptoFA::InstructionMetadata::AND_MASKED:
            boxcont << "Origine istruzione: Mascheratura di un AND\n";
            break;
        case NoCryptoFA::InstructionMetadata::CREATE_MASK:
            boxcont << "Origine istruzione: Inserimento maschera\n";
            break;
        case NoCryptoFA::InstructionMetadata::SHIFT_MASKED:
            boxcont << "Origine istruzione: Mascheratura di uno shift\n";
            break;
        case NoCryptoFA::InstructionMetadata::ORIGINAL_PROGRAM:
            boxcont << "Origine istruzione: Programma originale\n";
            break;
        case NoCryptoFA::InstructionMetadata::REMOVE_MASK:
            boxcont << "Origine istruzione: Rimozione maschera\n";
            break;
        case NoCryptoFA::InstructionMetadata::XOR_MASKED:
            boxcont << "Origine istruzione: Mascheratura di uno XOR\n";
            break;
        case NoCryptoFA::InstructionMetadata::OR_MASKED:
            boxcont << "Origine istruzione: Mascheratura di un OR\n";
            break;
        case NoCryptoFA::InstructionMetadata::CAST_MASKED:
            boxcont << "Origine istruzione: Mascheratura di un CAST\n";
            break;
        case NoCryptoFA::InstructionMetadata::SBOX_MASKED:
            boxcont << "Origine istruzione: Mascheratura di un lookup ad una SBOX\n";
            break;
        case NoCryptoFA::InstructionMetadata::SELECT_MASKED:
            boxcont << "Origine istruzione: Mascheratura di una SELECT\n";
            break;
        case NoCryptoFA::InstructionMetadata::MASKED_FUNCTION:
            boxcont << "Origine istruzione: Mascheratura di funzione intera\n";
            break;
    }

    boxcont << "Value size:" << md->pre.size() << "\n";
    if(!i->getDebugLoc().isUnknown()) {
        boxcont << "Nel sorgente a riga:" << i->getDebugLoc().getLine() << " colonna:" << i->getDebugLoc().getCol()  << "\n";
    }
    if(md->isAKeyOperation) {
        boxcont << "Keydep_Own:" << printbs_large<MAX_KEYBITS>(md->keydep_own) << "\nKeydep:" << printvec_large<MAX_KEYBITS>(md->keydep,cd->getMSBEverSet(),"keydep");
        boxcont << "Pre_Own:" << printbs_large<MAX_SUBBITS>(md->pre_own) << "\nPre:" << printvec_large<MAX_SUBBITS>(md->pre,cd->getMSBEverSet(),"pre");
        boxcont << "\nPre_Keydep:" << printvec_large<MAX_KEYBITS>(md->pre_keydep,cd->getMSBEverSet(),"pre-keydep");
        boxcont << "\nPost_Own:" << printbs_large<MAX_SUBBITS>(md->post_own) << "\nPost:" << printvec_large<MAX_SUBBITS>(md->post,cd->getMSBEverSet(),"post");
        boxcont << "\nPost_Keydep:" << printvec_large<MAX_KEYBITS>(md->post_keydep,cd->getMSBEverSet(),"post-keydep");
        boxcont << "\nFA_OutHits:" << printvec_large<MAX_OUTBITS>(md->out_hit,cd->getMSBEverSet(),"fa-outhits");
#ifdef OLD_FAULTS
        boxcont << "\nFA_Key_Own:" << printbs_large<MAX_KMBITS>(md->fullsubkey_own);
        boxcont << "\nFA_keydeps: Brace yourself, 3D slices are coming.\n";
        boxcont << "<div id=\"fa_bit\">BIT: min_nz:" << md->faultable_stats.min_keylen_nz << " oh :"<< md->faultable_stats.hw_outhit_of_min_keylen_nz << "\n";
        for(unsigned long i = 0; i < md->fault_keys.size(); i++){
            stringstream ss;
            ss << "faultkeys-bit-" << i;
            boxcont << printvec_large<MAX_KMBITS>(md->fault_keys[i],cd->getMSBEverSet_Fault(), ss.str()) << "\n";
            ss << "-kd";
            boxcont << printvec_large<MAX_KEYBITS>(md->fault_keys_keydep[i],cd->getMSBEverSet(), ss.str()) << "\n<hr/>\n";
        }
        boxcont << "</div><div id=\"fa_byte\">BYTE: min_nz:" << md->faultable_stats_byte.min_keylen_nz << " oh :"<< md->faultable_stats_byte.hw_outhit_of_min_keylen_nz << "\n";
        for(unsigned long i = 0; i < md->fault_keys_byte.size(); i++){
            stringstream ss;
            ss << "faultkeys-byte-" << i;
            boxcont << printvec_large<MAX_KMBITS>(md->fault_keys_byte[i],cd->getMSBEverSet_Fault(), ss.str()) << "\n";
            ss << "-kd";
            boxcont << printvec_large<MAX_KEYBITS>(md->fault_keys_keydep_byte[i],cd->getMSBEverSet(), ss.str()) << "\n<hr/>\n";
        }
        boxcont << "</div><div id=\"fa_word\">WORD: min_nz:" << md->faultable_stats_word.min_keylen_nz << " oh :"<< md->faultable_stats_word.hw_outhit_of_min_keylen_nz << "\n";
        for(unsigned long i = 0; i < md->fault_keys_word.size(); i++){
            stringstream ss;
            ss << "faultkeys-word-" << i;
            boxcont << printvec_large<MAX_KMBITS>(md->fault_keys_word[i],cd->getMSBEverSet_Fault(), ss.str()) << "\n";
            ss << "-kd";
            boxcont << printvec_large<MAX_KEYBITS>(md->fault_keys_keydep_word[i],cd->getMSBEverSet(), ss.str()) << "\n<hr/>\n";
        }
        boxcont << "</div>";
#endif
    }
    //md->pack();
    fname << md->NodeName << ".html";
    boxcont << "</pre></body></html>";
    outFile(fname.str(), boxcont.str());
    sem_post(&html_sem);
    return true;
}
#include <future>

void DFGPrinter::doHTML(Module& M){
    errs() << "Starting HTML output\n";
    list<future<bool> > operations;
    for(llvm::Module::iterator F = M.begin(), ME = M.end(); F != ME; ++F) {
        for(llvm::Function::iterator BB = F->begin(),
            FE = F->end();
            BB != FE;
            ++BB) {
            CalcDFG& cd = my_getAnalysis<CalcDFG>(F);
            if(!cd.functionMarked(&(*F))) { continue; }
            sem_init(&html_sem,0,1+std::thread::hardware_concurrency());
            for( llvm::BasicBlock::iterator i = BB->begin(); i != BB->end(); i++) {
                if(isa<llvm::DbgInfoIntrinsic>(i)) {continue;}
               operations.push_front(async(launch::async,doHTML_instruction,i,&cd));
             }
    }
  }
    for(future<bool> &v : operations){
        v.get();
    }
}
void DFGPrinter::doDOT(Module& M){
    MyNodeType* cur;
    bool added;
    errs() << "Starting DOT output\n";
    multimap<Instruction*, MyNodeType*> future_edges;
    for(llvm::Module::iterator F = M.begin(), ME = M.end(); F != ME; ++F) {
        MyNodeType* me = new MyNodeType(F->getName());
        rootptr->addChildren(me);
        instrnodemap.clear();
        future_edges.clear();
        for(llvm::Function::iterator BB = F->begin(),
            FE = F->end();
            BB != FE;
            ++BB) {
            CalcDFG& cd = my_getAnalysis<CalcDFG>(F);
            if(!cd.functionMarked(&(*F))) { continue; }
            for( llvm::BasicBlock::iterator i = BB->begin(); i != BB->end(); i++) {
                if(isa<llvm::DbgInfoIntrinsic>(i)) {continue;}
                std::string outp;
                outp.reserve(1024);
                llvm::raw_string_ostream os (outp);
                llvm::NoCryptoFA::InstructionMetadata* md = cd.getMD(i);
                os << md->getAsString() << "\n";
                if(md->isAKeyOperation) {
                    if(md->isAKeyStart) {
                        os << "KeyStart" << "\n";
                    }
                }
                cur = new MyNodeType(os.str());
                instrnodemap.insert(std::make_pair(i, cur));
                pair<multimap<Instruction*, MyNodeType*>::iterator, multimap<Instruction*, MyNodeType*>::iterator> range = future_edges.equal_range(i);
                for(multimap<Instruction*, MyNodeType*>::iterator it = range.first; it != range.second; ++it) {
                    cur->addChildren(it->second);
                }
                stringstream tmp;
                tmp << "Node" << cur;
                md->NodeName = tmp.str();
                cur->md = md;
                cur->hasToBeProtected = cd.shouldBeProtected(i);
                added = false;
                if(isa<PHINode>(i)) {
                    PHINode* p = cast<PHINode>(i);
                    for(unsigned int n = 0; n < p->getNumIncomingValues(); n++) {
                        if(isa<Instruction>(p->getIncomingValue(n))) {
                            Instruction* _it = cast<Instruction>(p->getIncomingValue(n));
                            if(instrnodemap.find(_it) != instrnodemap.end()) {
                                instrnodemap.at(_it)->addChildren(cur);
                                added = true;
                            } else {
                                future_edges.insert(std::make_pair(_it, cur));
                                // added=true; // Rischio "isole" sconnesse, che non apparirebbero nel grafo.
                            }
                        }
                    }
                } else {
                    for(User::const_op_iterator it = i->op_begin(); it != i->op_end(); ++it) {
                        if(isa<Instruction>(it->get())) {
                            Instruction* _it = cast<Instruction>(it->get());
                            if(instrnodemap.find(_it) != instrnodemap.end()) {
                                instrnodemap.at(_it)->addChildren(cur);
                                added = true;
                            } else {
                                future_edges.insert(std::make_pair(_it, cur));
                                // added=true; // Rischio "isole" sconnesse, che non apparirebbero nel grafo.
                            }
                        }
                    }
                }
                if(!added) { me->addChildren(cur); }
            }
        }
    }
}
bool DFGPrinter::runOnModule(llvm::Module& M){

    if(PrintDot) doDOT(M);
    if(PrintHTML) doHTML(M);
    if(PrintCSV) doCSV(M);
    doDAT(M);
    return false;
}

void DFGPrinter::getAnalysisUsage(llvm::AnalysisUsage& AU) const
{
	// Normally here we have to require analysis -- AU.addRequired -- and declare
	// preserved analysis -- AU.setPreserved. However, this pass does no require
	// any analysis and potentially invalidates all analysis. The default
	// behaviour is to invalidate all analysis.
//    AU.addRequired<TaggedData>();
//    AU.addRequiredTransitive<TaggedData>();
    AU.addRequired<CalcDFG>();


    AU.setPreservesCFG();
	AU.setPreservesAll();
}


DFGPrinter* llvm::createDFGPrinterPass()
{
	return new DFGPrinter();
}

using namespace llvm;


// The INITIALIZE_PASS_{BEGIN,END} macros generates some functions that can be
// used to register the pass to the LLVM pass registry.
// Parameters:
//
// HelloLLVM: pass class name
// "hello-llvm": command line switch to enable pass
// "Build an hello world": pass description
// false: the pass doesn't look only at the CFG
// false: the pass isn't an analysis.
INITIALIZE_PASS_BEGIN(DFGPrinter,
                      "dfgprint",
                      "dfgprint",
                      false,
                      false)
//INITIALIZE_PASS_DEPENDENCY(TaggedData)
INITIALIZE_PASS_DEPENDENCY(CalcDFG)

INITIALIZE_PASS_END(DFGPrinter,
                    "dfgprint",
                    "dfgprint",
                    false,
                    false)
