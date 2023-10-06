/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "eppcompiler.h"

//Write your code below this line

EPPCompiler::EPPCompiler(){

}

EPPCompiler::EPPCompiler(string out_file,int mem_limit){
    targ = Parser(); //check if this is required
    int memory_size = mem_limit;
    mem_loc.resize(mem_limit);
    output_file = out_file;
}

void EPPCompiler::compile(vector<vector<string>> code){
    int nlines = code.size();
    for (int i = 0; i< nlines; i++) {
        vector<string> currLine = code[i];

        if (currLine[0] == "del") {
            int delIndex = targ.symtable->search(currLine[2]);
            mem_loc.erase(mem_loc.begin(), mem_loc.begin() + delIndex);
        }

        else if (currLine[0] != "ret") {
            targ.symtable->assign_address(currLine[0], mem_loc.size());
        }

        //code for compilation
    }
}

vector<string> EPPCompiler::generate_targ_commands(){

}

void EPPCompiler::write_to_file(vector<string> commands){

}

EPPCompiler::~EPPCompiler(){
  
}
