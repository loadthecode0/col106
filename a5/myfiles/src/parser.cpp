/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "parser.h"

//Write your code below this line

Parser::Parser(){
    expr_trees = {}; //empty vector of parse trees
    symtable = new SymbolTable(); //empty symbol table initialized
}

void Parser::parse(vector<string> expression){

    ExprTreeNode* parseTree = new ExprTreeNode();
    parseTree->type = "EQ";
    parseTree->left = new ExprTreeNode();
    parseTree->right = new ExprTreeNode(); //3 nodes toh har case mein honge
    
    string prefix = expression[0];

    if (prefix == "del") {
        parseTree->left->type = "DEL";
        symtable -> remove(expression[2]);
    }

    else if (prefix == "ret"){
        parseTree->left->type = "RET";  //parseTree -> right untouched
    }

    else { //prefix is a variable name
        parseTree->left->type = "VAR";
        symtable -> insert(expression[0]); //does nothing if varname is already present in symtable
        parseTree->left->id = expression[0]; //parseTree -> right untouched
    }     

    ExprTreeNode* curr = parseTree->right;

    vector<ExprTreeNode*> parent = {parseTree};

    for (int i = 2; i<expression.size(); i++) {

        if (expression[i] == "(") {
            curr->left = new ExprTreeNode();
            parent.push_back(curr);
            curr = curr->left;
        }

        else if (expression[i] == "+") {
            curr->type = "ADD"; 
            curr->right = new ExprTreeNode();
            parent.push_back(curr);
            curr = curr->right;
        }

        else if (expression[i] == "-") {
            curr->type = "SUB"; 
            curr->right = new ExprTreeNode();
            parent.push_back(curr);
            curr = curr->right;
        }

        else if (expression[i] == "*") {
            curr->type = "MUL"; 
            curr->right = new ExprTreeNode();
            parent.push_back(curr);
            curr = curr->right;
        }

        else if (expression[i] == "/") {
            curr->type = "DIV"; 
            curr->right = new ExprTreeNode();
            parent.push_back(curr);
            curr = curr->right;
        }

        else if (expression[i] == "%") { 
            curr->type = "MOD"; 
            curr->right = new ExprTreeNode();
            parent.push_back(curr);
            curr = curr->right;
        }

        else if (expression[i].at(0) == '-' || ((int(expression[i].at(0)) >= 48) && ((int(expression[i].at(0)) <= 57))) ) {
            curr->type = "VAL";
            curr->num = stoi(expression[i]);
            curr = parent[parent.size() - 1];
            parent.pop_back();
        }

        else if (expression[i] == ")") {
            curr = parent[parent.size() - 1]; 
            parent.pop_back();
        }

        else {
            curr->type = "VAR"; 
            curr->id = expression[i];
            curr = parent[parent.size() - 1];
            parent.pop_back();
        }
    }
    expr_trees.push_back(parseTree);
}

void treeDelete (ExprTreeNode* root) {
    if (root == nullptr) {
        return;
    } 
    treeDelete (root->left);
    treeDelete (root->right);
    delete root; 
}

Parser::~Parser(){
    while(expr_trees.size() > 0) {
        ExprTreeNode* n = expr_trees[expr_trees.size() - 1];
        expr_trees.pop_back();
        treeDelete(n);
    }
    delete symtable;
}