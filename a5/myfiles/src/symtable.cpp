/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "symtable.h"

//Write your code below this line

SymbolTable::SymbolTable(){
    root = new SymNode(); //node with dummy value
    size = 0;
}

//utility function for recursive insertion
SymNode* entryInsert (SymNode* entry, string k) { //checked, works

    if (entry == nullptr) {
        return new SymNode(k);
    }

    if (k < entry->key) { //--1
        entry->left = entryInsert(entry->left, k);
    } else if (k > entry -> key) { //--2
        entry -> right = entryInsert(entry->right, k);
    } else { //key already exists
        return entry;
    }

    //this point is reached only when conditions 1 or 2 are met, ie one of the subtrees is modified
    //height balancing
    int balanceFactor = entry->left->height - entry->right->height;

    if (balanceFactor > 1) { //left subtree is longer than right by more than 1
        if (k < entry->left->key) { //insertion was in left subtree of left child of current node => left left imbalance
            return entry->RightRightRotation();
        }
        else if (k > entry->left->key) { //opp of above case
            return entry->LeftRightRotation();
        }
    }
    else if (balanceFactor < -1) { //right subtree is longer than left by more than 1
        if (k < entry->left->key) { //insertion was in left subtree of right child of current node => right left imbalance
            return entry->RightLeftRotation();
        }
        else if (k > entry->left->key) { //opp of above case
            return entry->LeftLeftRotation();
        }
    }
}

//utility function for recursive deletion, returns null if key DNE
//CHECK THIS
SymNode* entryDelete(SymNode* entry, string k) {
    if (entry == nullptr) {
        return entry;
    }

    if (entry->key > k) {
        entry -> left = entryDelete(entry->left, k);
        return entry;
    } else if (entry->key < k){ 
        entry->right = entryDelete(entry->right, k);
        return entry;
    }

    if (entry->left == nullptr) { //one or both children missing
        SymNode* temp = entry->right;
        delete entry; return temp;
    } else if (entry->right == nullptr) { //one child missing
        SymNode* temp = entry->left;
        delete entry; return temp;
    } else { 
        SymNode* p = entry;
        SymNode* s = entry->right;
        while (s->left != nullptr) {
            p = s; s=s->left;
        }
        if (p != entry) {
            p->left = s->right;
        } else {
            p->right = s->right;
        }

        entry->key = s->key;

        delete s; //delete successor
        return entry;

    }

    //height balancing
    int balanceFactor = entry->left->height - entry->right->height;

    if (balanceFactor > 1) { //left subtree is longer than right by more than 1
        if (k < entry->left->key) { //insertion was in left subtree of left child of current node => left left imbalance
            return entry->RightRightRotation();
        }
        else if (k > entry->left->key) { //opp of above case
            return entry->LeftRightRotation();
        }
    }
    else if (balanceFactor < -1) { //right subtree is longer than left by more than 1
        if (k < entry->left->key) { //insertion was in left subtree of right child of current node => right left imbalance
            return entry->RightLeftRotation();
        }
        else if (k > entry->left->key) { //opp of above case
            return entry->LeftLeftRotation();
        }
    }
}    

void SymbolTable::insert(string k){
    entryInsert(root, k);
    size++;
}

void SymbolTable::remove(string k){
    entryDelete(root, k);
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
    delete root; 
}

SymbolTable::~SymbolTable() {
    treeDelete(root);
}