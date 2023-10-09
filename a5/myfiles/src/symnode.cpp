/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "symnode.h"

//Write your code below this line

//DELETE THIS!!
#include <iostream>
using namespace std;
//DELETE THIS!!

SymNode::SymNode(){
    key = "newNode";
    height = 0;
    par = nullptr;
    left = nullptr;
    right = nullptr;
}

SymNode::SymNode(string k){
    key = k;
    height = 0;
    par = nullptr;
    left = nullptr;
    right = nullptr;
}

void updateHeight(SymNode* node) {
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

SymNode* SymNode::LeftLeftRotation(){ //for right-right imbalance, called for subtree rooted at 'this'
    SymNode* r = right;
    SymNode* rlc = r->left;
    //rotate
    if (par != nullptr) {
        if (key < par->key) { //ie current node is the left child of its parent
            par->left = r;
        } else { //ie current node is the right child of its parent
            par->right = r;
        }
        
    }
    r->par = par;
    
    right = rlc;

    if (rlc != nullptr) {
        rlc->par = this;
    }
    
    r->left = this;
    par = r;

    //height update
    updateHeight(this);
    updateHeight(r);
    if (r->par != nullptr) {updateHeight(r->par);}

    return r;
}

//checked RRRot, works
SymNode* SymNode::RightRightRotation(){ //for left-left imbalance, called for subtree rooted at 'this'
    SymNode* l = left;
    SymNode* lrc = l->right;
    //rotate
    if (par != nullptr) {
        if (key < par->key) { //ie current node is the left child of its parent
            par->left = l;
        } else { //ie current node is the right child of its parent
            par->right = l;
        }  
    }
    l->par = par;

    left = lrc;
    if (lrc != nullptr) {
        lrc->par = this;
    }
    
    l->right = this;
    par = l;

    updateHeight(this);
    updateHeight(l);
    if (l->par != nullptr) {updateHeight(l->par);}
    return l;
}

SymNode* SymNode::LeftRightRotation(){ //for left-right imbalance, called for subtree rooted at 'this'
    left->LeftLeftRotation();
    return RightRightRotation(); //height update implicit
}

SymNode* SymNode::RightLeftRotation(){ //for right-left imbalance, called for subtree rooted at 'this'
    right->RightRightRotation();
    return LeftLeftRotation(); //height update implicit
}

SymNode::~SymNode(){
    left = nullptr;
    right = nullptr;
    par = nullptr;
}

