/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "symtable.h"

//Write your code below this line

//DELETE THIS!!
#include <iostream>
using namespace std;
//DELETE THIS!!


SymbolTable::SymbolTable(){
    root = nullptr;
    size = 0;
}

//utility function for height update
void updateHt(SymNode* node) {
    //can be called for either null or non-null nodes
    if (node == nullptr) {
        return;
    }
    //precondition: node != nullptr
    int lh = -1; int rh = -1; //acc to nullptr condition
    if (node->left != nullptr) {
        lh = node->left->height;
    }
    if (node->right != nullptr) {
        rh = node->right->height;
    }
    node->height = max(lh, rh) + 1;
}

//utility function to find balance factor 
int bf (SymNode* entry) {
    int lh = 0; int rh = 0;

    if (entry -> left == nullptr) {
        lh = -1;
    } else {
        lh = entry->left->height;
    }

    if (entry -> right == nullptr) {
        rh = -1;
    } else {
        rh = entry->right->height;
    }

    return (lh-rh);
}

//utility function to balance height by rotations
SymNode* balanceHeight(SymNode* entry) {
    int balanceFactor = bf(entry);
    if (balanceFactor > 1) { //left subtree is longer than right by more than 1
        int bf1 = bf(entry->left);
        if (bf1 >= 0) { //left subtree of left child of current node is larger than right subtree => left left imbalance
            return entry ->RightRightRotation();
        }
        else { //opp of above case
            return entry->LeftRightRotation();
        }
    }
    else if (balanceFactor < -1) { //right subtree is longer than left by more than 1
        int bf1 = bf(entry->right);
        if (bf1 > 0) { //left subtree of right child of current node is larger than left subtree => right left imbalance
            return entry->RightLeftRotation();
        }
        else { //opp of above case
            return entry->LeftLeftRotation();
        }
    }
    return entry; //default return; hopefully we'll not reach here
}

//utility function for recursive insertion
SymNode* entryInsert (SymNode* entry, SymNode* parent, string k) { //checked, works

    if (entry == nullptr) {
        SymNode* x = new SymNode(k);
        x->par = parent;
        return x; //a leaf is always balanced
    }    

    if (k < entry->key) { //--1
        entry->left = entryInsert(entry->left, entry, k);
    } else if (k > entry -> key) { //--2
        entry -> right = entryInsert(entry->right, entry, k);
    } else { //key already exists
        return entry;
    }

    //this point is reached only when conditions 1 or 2 are met, ie one of the subtrees is modified
    //height balancing

    updateHt(entry);

    int balanceFactor = bf(entry);

    if (balanceFactor > 1) { //left subtree is longer than right by more than 1
        if (k < entry->left->key) { //insertion was in left subtree of left child of current node => left left imbalance
            return entry ->RightRightRotation();
        }
        else if (k > entry->left->key) { //opp of above case
            return entry->LeftRightRotation();
        }
    }
    else if (balanceFactor < -1) { //right subtree is longer than left by more than 1
        if (k < entry->right->key) { //insertion was in left subtree of right child of current node => right left imbalance
            return entry->RightLeftRotation();
        }
        else if (k > entry->right->key) { //opp of above case
            return entry->LeftLeftRotation();
        }
    }
    return entry; //default return; hopefully we'll not reach here
}

//utility function for recursive deletion, returns null if key DNE
//CHECK THIS
SymNode* entryDelete(SymNode* entry, string k) {

    if (entry == nullptr) {
        return entry;
    }

    if (entry->key > k) {
        entry -> left = entryDelete(entry->left, k);
        updateHt(entry);
        entry = balanceHeight(entry);
    } else if (entry->key < k){ 
        entry->right = entryDelete(entry->right, k);
        updateHt(entry);
        entry = balanceHeight(entry);
    } else if (k == entry -> key) {
        if (entry->left == nullptr || entry->right == nullptr) { //one child or no child

            SymNode* child = nullptr;
            if (entry->left != nullptr) {child = entry->left;}
            if (entry->right != nullptr) {child = entry->right;}

            if (entry->par != nullptr) {
                if (k < entry -> par -> key) {
                    entry -> par -> left = child;
                } else {
                    entry->par->right = child;
                }
                updateHt(entry->par);
            }

            if (child != nullptr) {
                child -> par = entry -> par;
                child = balanceHeight(child);  
            }

            delete entry;       
            return child;
        }

        else {
            SymNode* temp = entry -> right;
            while (temp -> left != nullptr) {
                temp = temp->left;
            }
            string copyKey = temp->key;
            int copyAdd = temp->address;
            temp = entryDelete(temp, copyKey);
            updateHt(entry);
            entry->key = copyKey; //replace
            entry->address = copyAdd;
            entry = balanceHeight(entry);
        }
        return entry;
    }
    return entry;
}    

void SymbolTable::insert(string k){ //checked, works
    //pre condition: not already present in the table
    root = entryInsert(root, nullptr, k);
    size++;
}

void SymbolTable::remove(string k){
    root = entryDelete(root, k);
    size--;
}

//utility function for recursive search
SymNode* entrySearch (SymNode* entry, string k) {
    if (entry->key == k) {
        return entry;
    }

    if (k < entry->key) {
        return entrySearch(entry->left, k);
    }

    if (k > entry->key) {
        return entrySearch(entry->right, k);
    }
    return nullptr; //default
}

int SymbolTable::search(string k){
    return entrySearch(root, k)->address;
}

void SymbolTable::assign_address(string k,int idx){
    entrySearch(root, k)->address = idx;
}

int SymbolTable::get_size(){
    return size;
}

SymNode* SymbolTable::get_root(){
    return root;
}

//utility function for recursive deletion of full tree
void treeDelete (SymNode* root) {
    if (root == nullptr) {
        return;
    } 
    treeDelete (root->left);
    treeDelete (root->right);
    delete root; root = nullptr;
}

SymbolTable::~SymbolTable() {
    treeDelete(root);
}

// int main() {
    
//     SymbolTable* s = new SymbolTable();
//     cout << "size: " << s->get_size() << "\n";
    
//     s->insert("a");
//     s->insert("b");
//     s->insert("c");
//     s->insert("e");
//     s->insert("d");

//     cout << "size: " << s->get_size() << "\n";
//     cout << s->get_root()->key << "\n";
//     cout << s->get_root()->left->key << "\n";
//     cout << s->get_root()->right->key << "\n";
//     cout << s->get_root()->right->right->key << "\n";
//     cout << s->get_root()->right->left->key << "\n";
//     cout << "size: " << s->get_size() << "\n";
//     // s->remove("a");
//     // cout << s->get_root()->key << "\n";
//     // cout << "size: " << s->get_size() << "\n";
//     // cout << s->get_root()->left->key << "\n";
//     s->remove("a");
//     cout << s->get_root()->key << "\n";
//     cout << s->get_root()->left->key << "\n";
//     cout << s->get_root()->right->key << "\n";
//     cout << s->get_root()->left->right->key << "\n";
//     // cout << s->get_root()->right->left->key << "\n";
//     cout << "size: " << s->get_size() << "\n";

//     return 0;
// }