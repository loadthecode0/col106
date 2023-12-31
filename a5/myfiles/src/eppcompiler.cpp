/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "eppcompiler.h"

//Write your code below this line

EPPCompiler::EPPCompiler(){

}

EPPCompiler::EPPCompiler(string out_file,int mem_limit){
    int memory_size = mem_limit;
    // mem_loc.resize(memory_size); //make and initialize list of empty memory locations
    // for (int i = 0; i<memory_size; i++) {
    //     mem_loc[i] = i;
    // }
    for (int i = 0; i<memory_size; i++) { //make and initialize MinHeap of empty memory locations
        least_mem_loc.push_heap(i);
    }

    output_file = out_file;
}

void EPPCompiler::compile(vector<vector<string>> code){
    int nlines = code.size();
    vector<string>f_commds;
    for (int i = 0; i< nlines; i++) {
        vector<string> currLine = code[i]; //read current line
        targ.parse(currLine); //parse current line

        if (currLine[0] == "del") {
            int delAddress = targ.last_deleted; //returns memory address last deleted
            least_mem_loc.push_heap(delAddress);
        }
        else if (currLine[0] != "ret") { //variable
            int varAddress = targ.symtable->search(currLine[0]); 
            if (varAddress == -1) { //address not assigned before, ie, new variable
                targ.symtable->assign_address(currLine[0], least_mem_loc.get_min());
                least_mem_loc.pop();
            }
        }

        //generation of targ string for current line
        vector<string> outputLine = generate_targ_commands();

        for (const string & x : outputLine) {
            f_commds.push_back(x);
        }
        
    }
    //writing to output file
    write_to_file(f_commds);
}

//helper function for modified post order traversal of parse tree
void traverseInsert (Parser &targ, ExprTreeNode* root, vector<string> &v, SymbolTable * table) {
    if (root == NULL) {
        return;
    }

    traverseInsert (targ, root->right, v, table);
    traverseInsert (targ, root->left, v, table);
    //for current ie root
    string s;
    if (root->type == "VAL") {
        s = "PUSH " + to_string(root -> num);
    }

    else if (root->type == "VAR") {
        int address = table->search(root->id);
        s = "PUSH mem[" + to_string(address) + "]";
    }

    else {
        s = root->type;
    }

    v.push_back(s);
}

vector<string> EPPCompiler::generate_targ_commands(){
    ExprTreeNode* parseTree = targ.expr_trees.back();
    vector<string> v = {};

    ExprTreeNode* prefixNode = parseTree->left;

    if (prefixNode->type == "DEL") {
        v.push_back("DEL = mem[" + to_string(targ.last_deleted) + "]");
    }

    else if (prefixNode->type == "RET") {
        traverseInsert(targ, parseTree->right, v, targ.symtable); //till now, v contains the vector of strings corresponding to current parsetree
        v.push_back("RET = POP");
    }

    else {
        traverseInsert(targ, parseTree->right, v, targ.symtable); //till now, v contains the vector of strings corresponding to current parsetree
        int address = targ.symtable->search(prefixNode->id);
        v.push_back("mem[" + to_string(address) + "] = POP");
    }
    
    return v;
}

void EPPCompiler::write_to_file(vector<string> commands){
    ofstream gen_file;
    gen_file.open(output_file);
    for (string x:commands) {
        gen_file << x << "\n";
    }
    gen_file.close();
}

EPPCompiler::~EPPCompiler(){

}
