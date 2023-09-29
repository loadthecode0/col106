/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "evaluator.h"

//DELETE THESE!
#include <iostream>
using namespace std;
//DELETE THESE!

Evaluator::Evaluator() {
    expr_trees = {}; //empty vector of parse trees
    symtable = new SymbolTable(); //empty symbol table initialized
}

Evaluator::~Evaluator() {
    expr_trees.clear();
    delete symtable;
}

void Evaluator::parse(vector<string> code) {
    std::cout << "Entered Parser\n";

    std::cout << "Tokens: \n";
    for (string x:code) {
        cout << x << " ";
    }
    cout << "\n";

    ExprTreeNode* parseTree = new ExprTreeNode();
    parseTree->type = "EQ";
    
    parseTree->left = new ExprTreeNode("VAR", new UnlimitedInt(0));
    // parseTree->left->type = "VAR";
    parseTree->left->id = code[0];

    parseTree->right = new ExprTreeNode();
    ExprTreeNode* curr = parseTree->right;
    std::cout <<"initiation, type: " << curr->type << "\n";

    vector<ExprTreeNode*> parent = {parseTree};

    for (int i = 2; i<code.size(); i++) {

        if (code[i] == "(") {
            std::cout <<"token: "<<code[i] << ", type: " << curr->type << "\n";
            curr->left = new ExprTreeNode();
            parent.push_back(curr);
            curr = curr->left;
        }

        // else if (code[i] == ":=") {
        //     curr->type = "EQ"; std::cout <<"token: "<<code[i] << ", type: " << curr->type << "\n";
        //     curr->right = new ExprTreeNode();
        //     curr->type = "EQ"; std::cout <<"token: "<<code[i] << ", type: " << curr->type << "\n";
        //     cout << "hello\n";
        //     parent[0] = curr;
        //     cout << "hello2\n";
        //     curr = curr->right;
        //     cout << "hello3\n";
        // }

        else if (code[i] == "+") {
            curr->type = "ADD"; std::cout <<"token: "<<code[i] << ", type: " << curr->type << "\n";
            curr->right = new ExprTreeNode();
            parent.push_back(curr);
            curr = curr->right;
        }

        else if (code[i] == "-") {
            curr->type = "SUB"; std::cout <<"token: "<<code[i] << ", type: " << curr->type << "\n";
            curr->right = new ExprTreeNode();
            parent.push_back(curr);
            curr = curr->right;
        }

        else if (code[i] == "*") {
            curr->type = "MUL"; std::cout <<"token: "<<code[i] << ", type: " << curr->type << "\n";
            curr->right = new ExprTreeNode();
            parent.push_back(curr);
            curr = curr->right;
        }

        else if (code[i] == "/") {
            curr->type = "DIV"; std::cout <<"token: "<<code[i] << ", type: " << curr->type << "\n";
            curr->right = new ExprTreeNode();
            parent.push_back(curr);
            curr = curr->right;
        }

        else if (code[i] == "%") { 
            curr->type = "MOD"; std::cout <<"token: "<<code[i] << ", type: " << curr->type << "\n";
            curr->right = new ExprTreeNode();
            parent.push_back(curr);
            curr = curr->right;
        }

        else if (code[i].at(0) == '-' || ((int(code[i].at(0)) >= 48) && ((int(code[i].at(0)) <= 57))) ) {
            curr->type = "VAL";
            curr->val = new UnlimitedRational(new UnlimitedInt(code[i]), new UnlimitedInt("1"));
            std::cout <<"token: "<<code[i] << ", type: " << curr->type << ", value = " << curr->val->get_frac_str()<< "\n";
            curr = parent[parent.size() - 1];
            parent.pop_back();
        }

        else if (code[i] == ")") {
            std::cout <<"token: "<<code[i] << ", type: " << curr->type << "\n";
            curr = parent[parent.size() - 1]; 
            parent.pop_back();
        }

        else {
            curr->type = "VAR"; std::cout << ", type: " << curr->type << "\n";
            curr->id = code[i];
            curr = parent[parent.size() - 1];
            parent.pop_back();
        }
        std::cout << ", AFTER, type: " << curr->type << "\n"; 
    }
    expr_trees.push_back(parseTree);
    std::cout << "Completed parsing\n";
}

UnlimitedRational* evalUtil(ExprTreeNode* curr, SymbolTable* symtable) {
    cout << curr->type << "\n";
    // cout << curr->left << " " << curr->right << "\n";
    // cout << curr->left->id << "\n";
    // cout << curr->right->type << "\n";
    // cout << curr->right->left->val->get_frac_str() << "\n";
    // cout << curr->right->right->val->get_frac_str()<< "\n";
    

    if ((curr->left == nullptr) && (curr->right == nullptr)) { //leaf node
        cout << "hello1\n";
        cout << "Leaf node reached\n"; 
        if (curr->type == "VAL") {
            curr->evaluated_value = curr->val;
        }
        else if (curr->type == "VAR") { //not for top variable (associated with :=)
            cout << "need to access symbol table\n";
            cout << curr->id << "\n";
            curr->evaluated_value = symtable->search(curr->id);

        }
        std::cout << "eval_val = " << curr->evaluated_value->get_frac_str() << "\n";
        return curr->evaluated_value;
    }

    else { //operator node
        cout << "hello2\n";
        if (curr->type == "EQ") {
            string varName = curr->left->id;
            symtable->insert(varName,curr->right->evaluated_value);
            // return evalUtil(curr->right, symtable);
        }

        else if (curr->type == "ADD") {
            cout << "hello3\n";
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
        std::cout << "eval_val = " << curr->evaluated_value->get_frac_str() << "\n";
        return curr->evaluated_value;

        // else if (curr->type == "MOD") {
        //     curr->evaluated_value = UnlimitedRational::mod(curr->left->evaluated_value, curr->right->evaluated_value);
        // }
    }
    std::cout << "Completed evaluating\n";
    // std::cout << curr->type << " " << currVal->get_frac_str() << "\n";
    // std::cout << symtable->search(curr->left->id)->get_frac_str() << "\n";
}

void Evaluator::eval() {

    std::cout << "Entered main Evaluator\n";
    std::cout << expr_trees.size() << "\n";

    ExprTreeNode *parseTree = expr_trees[expr_trees.size()-1];
    ExprTreeNode *x = parseTree->right;

    UnlimitedRational* y = evalUtil(x, symtable);
    symtable->insert(parseTree->left->id, y);
    cout << y->get_frac_str() << "\n";
    // UnlimitedRational* currVal = new UnlimitedRational();

    // ExprTreeNode* curr = expr_trees[expr_trees.size() - 1];
    // //Notice: leaves can only be VAL or VAR types.
    // //Only operators can have children nodes, by construction

    // if ((curr->left == nullptr) && (curr->right == nullptr)) { //leaf node
    //     if (curr->type == "VAL") {
    //         curr->evaluated_value = curr->val;
    //     }
    //     else if (curr->type == "VAR") { //not for top variable (associated with :=)
    //         curr->evaluated_value = symtable->search(curr->id);
    //     }
    //     currVal = curr->evaluated_value;
    // }

    // else { //operator node
    //     if (curr->type == "EQ") {
    //         string varName = curr->left->id;
    //         symtable->insert(varName,curr->right->evaluated_value);
    //     }

    //     else if (curr->type == "ADD") {
    //         curr->evaluated_value = UnlimitedRational::add(curr->left->evaluated_value, curr->right->evaluated_value);
    //     }

    //     else if (curr->type == "SUB") {
    //         curr->evaluated_value = UnlimitedRational::sub(curr->left->evaluated_value, curr->right->evaluated_value);
    //     }

    //     else if (curr->type == "MUL") {
    //         curr->evaluated_value = UnlimitedRational::mul(curr->left->evaluated_value, curr->right->evaluated_value);
    //     }

    //     else if (curr->type == "DIV") {
    //         curr->evaluated_value = UnlimitedRational::div(curr->left->evaluated_value, curr->right->evaluated_value);
    //     }

    //     currVal = curr->evaluated_value;

    //     // else if (curr->type == "MOD") {
    //     //     curr->evaluated_value = UnlimitedRational::mod(curr->left->evaluated_value, curr->right->evaluated_value);
    //     // }
    // }
    std::cout << "Completed main evaluating\n";
    // std::cout << curr->type << " " << currVal->get_frac_str() << "\n";
    // std::cout << symtable->search(curr->left->id)->get_frac_str() << "\n";
}