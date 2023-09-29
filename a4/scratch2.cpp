#include <bits/stdc++.h>
using namespace std;

/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include <string>

class SymEntry {
public:
    string key;
    int val;
    
    SymEntry* left;
    SymEntry* right;

    SymEntry();
    SymEntry(string k, int v);

    ~SymEntry();
    
};

SymEntry::SymEntry() {
    key = "newEntry"; //new root key approx in the middle of alphabet
    val = 0;
    left = nullptr;
    right = nullptr;
}

SymEntry::SymEntry(string k, int v) {
    key = k;
    val = v;
    left = nullptr;
    right = nullptr;
}

SymEntry::~SymEntry() {
    delete left; left = nullptr;
    delete right; right = nullptr;
}

class SymbolTable {
private:
    int size;
    SymEntry* root;

public: 

    SymbolTable();

    ~SymbolTable();

    // Insert a key value pair in the symbol table
    void insert(string k, int v);

    // Remove a key (and value) in the symbol table
    void remove(string k);
 
    // Find the val corresponding to the key in the symbol table
    int search(string k);

    // Get size
    int get_size();

    // Get root 
    SymEntry* get_root();

    void display();
};

SymbolTable::SymbolTable() {
    root = new SymEntry(); //-----------------changed!!!
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

SymEntry* entryInsert (SymEntry* entry, string k, int v) {
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

void SymbolTable::insert(string k, int v) {
    entryInsert(root, k, v);
    size++;
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

int SymbolTable::search(string k){
    SymEntry* curr = root;
    while (curr!=nullptr && curr->key != k) {
        if (k < curr->key) {
            curr = curr->left;
        } else { //k>curr->key
            curr = curr->right;
        }
        cout << curr->key << " " << curr->val << "\n";
    } //curr = nullptr or required node now; test cases will be valid
    if (curr->key == k) {
        return curr->val;
    }    
}

int SymbolTable::get_size(){
    return size;
}

SymEntry* SymbolTable::get_root(){
    return root;
}

void inorder (SymEntry* root) {
    if (!root) {
        return;
    } 

    inorder(root->left);
    cout << "key: " << root->key << ", value: " << root->val << "\n";
    inorder(root->right);
}

void SymbolTable::display() {
    inorder(root);
}

int main()
{
    SymbolTable* s = new SymbolTable();
    s->insert("x", 9);
    s->insert("a", 5);
    s->insert("v", 3);
    s->insert("w", 0);
    

    s->display();

    cout << s->search("v") << "\n";


    
    return 0;
}