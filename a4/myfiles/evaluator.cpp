/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "evaluator.h"

Evaluator::Evaluator() {
    expr_trees = {}; //empty vector of parse trees
    symtable = new SymbolTable(); //empty symbol table initialized
}

//utility function for recursive deletion of full tree
void treeDelete (ExprTreeNode* root) {
    if (root == nullptr) {
        return;
    } 
    treeDelete (root->left);
    treeDelete (root->right);
    if (root->val != nullptr) {delete root-> val;}
    if (root->evaluated_value != nullptr) {delete root->evaluated_value;}
    delete root; 
}

Evaluator::~Evaluator() {
    while(expr_trees.size() > 0) {
        ExprTreeNode* n = expr_trees[expr_trees.size() - 1];
        expr_trees.pop_back();
        treeDelete(n);
    }
    delete symtable;
}

void Evaluator::parse(vector<string> code) {

    ExprTreeNode* parseTree = new ExprTreeNode();
    parseTree->type = "EQ";
    
    parseTree->left = new ExprTreeNode();
    parseTree->left->type = "VAR";
    parseTree->left->id = code[0];

    parseTree->right = new ExprTreeNode();
    ExprTreeNode* curr = parseTree->right;

    vector<ExprTreeNode*> parent = {parseTree};

    for (int i = 2; i<code.size(); i++) {

        if (code[i] == "(") {
            curr->left = new ExprTreeNode();
            parent.push_back(curr);
            curr = curr->left;
        }

        else if (code[i] == "+") {
            curr->type = "ADD"; 
            curr->right = new ExprTreeNode();
            parent.push_back(curr);
            curr = curr->right;
        }

        else if (code[i] == "-") {
            curr->type = "SUB"; 
            curr->right = new ExprTreeNode();
            parent.push_back(curr);
            curr = curr->right;
        }

        else if (code[i] == "*") {
            curr->type = "MUL"; 
            curr->right = new ExprTreeNode();
            parent.push_back(curr);
            curr = curr->right;
        }

        else if (code[i] == "/") {
            curr->type = "DIV"; 
            curr->right = new ExprTreeNode();
            parent.push_back(curr);
            curr = curr->right;
        }

        else if (code[i] == "%") { 
            curr->type = "MOD"; 
            curr->right = new ExprTreeNode();
            parent.push_back(curr);
            curr = curr->right;
        }

        else if (code[i].at(0) == '-' || ((int(code[i].at(0)) >= 48) && ((int(code[i].at(0)) <= 57))) ) {
            curr->type = "VAL";
            curr->val = new UnlimitedRational(new UnlimitedInt(code[i]), new UnlimitedInt("1"));
            curr = parent[parent.size() - 1];
            parent.pop_back();
        }

        else if (code[i] == ")") {
            curr = parent[parent.size() - 1]; 
            parent.pop_back();
        }

        else {
            curr->type = "VAR"; 
            curr->id = code[i];
            curr = parent[parent.size() - 1];
            parent.pop_back();
        }
    }
    expr_trees.push_back(parseTree);
}

UnlimitedRational* evalUtil(ExprTreeNode* curr, SymbolTable* symtable) {    

    if ((curr->left == nullptr) && (curr->right == nullptr)) { //leaf node
        if (curr->type == "VAL") {
            curr->evaluated_value = curr->val;
        }
        else if (curr->type == "VAR") { //not for top variable (associated with :=)
            curr->evaluated_value = symtable->search(curr->id);

        }
        return curr->evaluated_value;
    }

    else { //operator node
        if (curr->type == "EQ") {
            string varName = curr->left->id;
            symtable->insert(varName,curr->right->evaluated_value);
        }

        else if (curr->type == "ADD") {
            curr->evaluated_value = UnlimitedRational::add(evalUtil(curr->left, symtable), evalUtil(curr->right, symtable));
        }

        else if (curr->type == "SUB") {
            curr->evaluated_value = UnlimitedRational::sub(evalUtil(curr->left, symtable), evalUtil(curr->right, symtable));
        }

        else if (curr->type == "MUL") {
            curr->evaluated_value = UnlimitedRational::mul(evalUtil(curr->left, symtable), evalUtil(curr->right, symtable));
        }
        else if (curr->type == "DIV") {
            curr->evaluated_value = UnlimitedRational::div(evalUtil(curr->left, symtable), evalUtil(curr->right, symtable));
        }
        return curr->evaluated_value;
    }
}

void Evaluator::eval() {
    ExprTreeNode *parseTree = expr_trees[expr_trees.size()-1];
    ExprTreeNode *x = parseTree->right;

    UnlimitedRational* y = evalUtil(x, symtable);
    symtable->insert(parseTree->left->id, y);
}