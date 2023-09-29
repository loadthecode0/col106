/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "symtable.h"

//DELETE THESE!
#include <iostream>
using namespace std;
//DELETE THESE!

SymbolTable::SymbolTable() {
    root = new SymEntry();
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

SymEntry* entryInsert (SymEntry* entry, string k, UnlimitedRational* v) { //checked, works
    cout<<"helloinsert " << k <<"is the key\n" ;

    if (entry == nullptr) {
        return new SymEntry(k ,v);
    }

    if (k < entry->key) {
        entry->left = entryInsert(entry->left, k, v);
    } else if (k > entry -> key) {
        entry -> right = entryInsert(entry->right, k, v);
    }

    return entry;
}

void SymbolTable::insert(string k, UnlimitedRational* v) {
    SymEntry* e = entryInsert(root, k, v);
    size++;
    if (root->left) {
        cout << root->left->key << "\n";
    }
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
    size--;
}

SymEntry* entrySearch (SymEntry* entry, string k) {
    if (entry->key == k) {
        return entry;
    }

    if (k < entry->key) {
        return entrySearch(entry->left, k);
    }

    if (k > entry->key) {
        return entrySearch(entry->right, k);
    }
}

UnlimitedRational* SymbolTable::search(string k){
    std::cout << "hello search" << " varname: " << k << "\n";
    SymEntry* curr = root;
    // cout << curr->key << " " << curr->val->get_frac_str() << "\n";
    // cout << curr->right->key << " " << curr->right->val->get_frac_str() << "\n";
    return entrySearch(root, k)->val;
    // while (curr!=nullptr) {
    //     cout << curr->key << " x " << curr->val->get_frac_str() << "\n";
    //     while (k < curr->key && curr->left != nullptr) {
            
    //         cout << curr->key << " l " << curr->val->get_frac_str() << "\n";
    //         curr = curr->left;
    //     } 
        
    //     while (k > curr->key){ //k>curr->key
            
    //         cout << curr->key << " r " << curr->val->get_frac_str() << "\n";
    //         curr = curr->right;
    //         cout<<"hello"<<"\n";
    //     }

    //     if (curr->key == k) {
    //     return curr->val;
    //     }
    //     cout << curr->key << " y " << curr->val->get_frac_str() << "\n";
    // } //curr = nullptr or required node now; but test cases will be valid    
}

int SymbolTable::get_size(){
    return size;
}

SymEntry* SymbolTable::get_root(){
    return root;
}