/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "symnode.h"

//Write your code below this line

SymNode::SymNode(){
    key = "newNode";
    height = 1;
    par = nullptr;
    left = nullptr;
    right = nullptr;
}

SymNode::SymNode(string k){
    key = k;
    height = 1;
    par = nullptr;
    left = nullptr;
    right = nullptr;
}

SymNode* SymNode::LeftLeftRotation(){ //for right-right imbalance, called for subtree rooted at 'this'
    SymNode* r = right;
    SymNode* rlc = r->left;
    //rotate
    if (key < par->key) { //ie current node is the left child of its parent
        par->left = r;
    } else { //ie current node is the right child of its parent
        par->right = r;
    }
    r->par = par;
    right = rlc;
    rlc->par = this;
    r->left = this;
    par = r;

    //height update
    height = max(left->height, right->height) + 1;
    r->height = max(r->left->height, r->right->height) + 1;
    par->height = max(par->left->height, par->right->height) + 1;

    return r;
}

SymNode* SymNode::RightRightRotation(){ //for left-left imbalance, called for subtree rooted at 'this'
    SymNode* l = left;
    SymNode* lrc = l->right;
    //rotate
    if (key < par->key) { //ie current node is the left child of its parent
        par->left = l;
    } else { //ie current node is the right child of its parent
        par->right = l;
    }
    l->par = par;
    left = lrc;
    lrc->par = this;
    l->right = this;
    par = l;

    //height update
    height = max(left->height, right->height) + 1;
    l->height = max(l->left->height, l->right->height) + 1;
    par->height = max(par->left->height, par->right->height) + 1;

    return l;
}

SymNode* SymNode::LeftRightRotation(){ //for left-right imbalance, called for subtree rooted at 'this'
    SymNode* x = left->LeftLeftRotation();
    return RightRightRotation(); //height update implicit
}

SymNode* SymNode::RightLeftRotation(){ //for right-left imbalance, called for subtree rooted at 'this'
    SymNode* x = right->RightRightRotation();
    return LeftLeftRotation(); //height update implicit
}

SymNode::~SymNode(){
    delete par;
    delete left;
    delete right;    
}