/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "symtable.h"

SymbolTable::SymbolTable() {
    root = nullptr;
    size = 0;
}

//utility function for recursive deletion of full tree
void treeDelete (SymEntry* root) {
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

void SymbolTable::insert(string k, UnlimitedRational* v) {
    SymEntry* curr = root;
    while (curr!=nullptr) {
        if (k < curr->key) {
            curr = curr->left;
        } else { //k>curr->key
            curr = curr->right;
        }
    } //curr = nullptr now
    curr = new SymEntry(k, v);
}

SymEntry* entryDelete(SymEntry* entry, string k) {
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
        SymEntry* temp = entry->right;
        delete entry; return temp;
    } else if (entry->right == nullptr) { //one child missing
        SymEntry* temp = entry->left;
        delete entry; return temp;
    } else {
        SymEntry* p = entry;
        SymEntry* s = entry->right;
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
}    

void SymbolTable::remove(string k){
    entryDelete(root, k);
}

UnlimitedRational* SymbolTable::search(string k){
    SymEntry* curr = root;
    while (curr!=nullptr && curr->key != k) {
        if (k < curr->key) {
            curr = curr->left;
        } else { //k>curr->key
            curr = curr->right;
        }
    } //curr = nullptr or required node now; test cases will be valid
    return curr->val;
}

int SymbolTable::get_size(){
    return size;
}

SymEntry* SymbolTable::get_root(){
    return root;
}