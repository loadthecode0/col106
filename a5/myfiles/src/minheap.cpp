/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "minheap.h"

//Write your code below this line

//helper function to convert decimal to binary
string myBin(int n) {
    int t = n;
    string s = "";
    while (t > 0) {
        s = to_string(t%2) + s;
        t /= 2;
    }
    return s;
}

MinHeap::MinHeap(){
    //nothing required really
}

HeapNode* findLast(HeapNode* root, int n) { //n = size of tree
    string s = myBin(n);
    HeapNode* currNode = root;
    for (int i = 1; i < s.length(); i++) {
        if (s[i] == '0') {
            currNode = currNode -> left;
        } else {
            currNode = currNode -> right;
        }
    }
    return currNode;
}

HeapNode* findParentInsert(HeapNode* root, int n) {
    string s = myBin(n+1);
    // HeapNode* prevNode = NULL;
    HeapNode* currNode = root;
    for (int i = 1; i < (s.length() - 1); i++) {
        // prevNode = currNode;
        if (s[i] == '0') {
            currNode = currNode -> left;
        } else {
            currNode = currNode -> right;
        }
    }
    return currNode;
}

void MinHeap::push_heap(int num){
    
    if (root == NULL) {
        root = new HeapNode(num);
        size++;
        return;
    }
    HeapNode* x = findParentInsert(root, size);
    HeapNode* y = NULL;
    //pre condition: root is not null, ie atleast one non null node here, size>=1
    if (x->left == NULL) {
        x->left = new HeapNode(num);
        x->left->par = x;
        y = x->left;
    } else {
        x->right = new HeapNode(num);
        x->right->par = x;
        y = x->right;
    }
    size++;

    //bubble up
    while (y->par != NULL && y->val < y->par->val) {
        //swap
        int tempVal = y->par->val;
        y->par->val = y->val;
        y->val = tempVal;
        //move pointer up
        y = y->par;
    }
}

int MinHeap::get_min(){
    return root->val;
}

void MinHeap::pop(){
    if (root == NULL) {
        return;
    }

    if (root->left == NULL && root -> right == NULL) {
        delete root; root = NULL;
        return;
    }

    //now we have multiple layers

    HeapNode* x = findLast(root, size);
    //copy last value to root
    root->val = x->val;

    //delete last node;
    if (x->par != NULL) {
        if (x == x->par->left) {
            x->par->left = NULL;
        } else {
            x->par->right = NULL;
        }
    }     
    x->par = NULL;
    delete x; x = NULL;
    size--;
    
    //bubble down
    HeapNode* currNode = root;
    HeapNode* min = NULL;
    while (!(currNode->left == NULL && currNode->right == NULL)) { //ie not a leaf
        //not a leaf => at least one child exists. But since heap is left filled, left child def exists. Right may or may not.
        //break if all existing children have greater values

        if (currNode->right == NULL) { //only left child exists
            if (currNode->left->val > currNode-> val) {
                break;
            }
        }
        else { //both children exist
            if ((currNode->left->val > currNode-> val) && (currNode->right->val > currNode-> val) ) {
                break;
            }
        }

        if (currNode->left != NULL && currNode->left->val < currNode->val) {
            min = currNode->left;
        } 
        
        if (currNode->right != NULL && currNode->right->val < currNode->left->val) {
            min = currNode->right;
        }
    
        int tempVal = min->val;
        min->val = currNode->val;
        currNode->val = tempVal;
        
        currNode = min;
    }
}

void treeDelete (HeapNode* root) {
    if (root == NULL) {
        return;
    } 
    treeDelete (root->left);
    treeDelete (root->right);
    delete root; root = NULL;
}

MinHeap::~MinHeap(){
    treeDelete(root);
}

// int main() {

//     MinHeap* m = new MinHeap();
//     m->push_heap(9);
//     m->push_heap(10);
//     m->push_heap(1);
//     m->push_heap(-10);
//     m->push_heap(100);
//     m->push_heap(23);
//     m->push_heap(-46);
//     cout << m->get_min() << "\n";
//     cout << "...\n";
//     cout << m->root->left->par->val << "\n";
//     cout << m->root->right->par->val << "\n";
//     cout << m->root->left->par->left->val << "\n";
//     cout << m->root->left->right->par->val << "\n";
//     cout << m->root->right->left->par->val << "\n";
//     cout << m->root->right->right->par->val << "\n";
//     cout << "...\n";
//     m->pop();
//     cout << m->get_min() << "\n";
//     cout << m->root->left->val << "\n";
//     cout << m->root->right->val << "\n";
//     cout << m->root->left->left->val << "\n";
//     cout << m->root->left->right->val << "\n";
//     cout << m->root->right->left->val << "\n";
//     // cout << m->root->right->right->val << "\n";
//     cout << "...\n";
//     m->pop();
//     cout << m->get_min() << "\n";
//     cout << m->root->left->val << "\n";
//     cout << m->root->right->val << "\n";
//     cout << m->root->left->left->val << "\n";
//     cout << m->root->left->right->val << "\n";
//     cout << "...\n";
//     cout << m->get_min() << "\n";
//     cout << m->root->left->par->val << "\n";
//     cout << m->root->right->par->val << "\n";
//     cout << m->root->left->left->par->val << "\n";
//     cout << m->root->left->right->par->val << "\n";
//     cout << "...\n";
//     m->pop();
//     cout << m->get_min() << "\n";
//     cout << m->root->left->val << "\n";
//     cout << m->root->right->val << "\n";
//     cout << m->root->left->left->val << "\n";
//     delete m;

//     return 0;
// }