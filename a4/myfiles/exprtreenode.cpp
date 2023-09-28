/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "exprtreenode.h"

ExprTreeNode::ExprTreeNode() {
    type = ""; //dummy value
    val = new UnlimitedRational();
    evaluated_value = new UnlimitedRational;
    left = nullptr;
    right = nullptr;
}

ExprTreeNode::ExprTreeNode(string t, UnlimitedInt* v) {
    type = t;
    val = new UnlimitedRational(v, new UnlimitedInt("1"));
}

ExprTreeNode::ExprTreeNode(string t, UnlimitedRational* v) {
    type = t;
    val = v;
}

//utility function for recursive deletion of full tree
void treeDelete (ExprTreeNode* root) {
    if (root == nullptr) {
        return;
    } 
    treeDelete (root->left);
    treeDelete (root->right);
    delete root; 
}

ExprTreeNode::~ExprTreeNode() {
    treeDelete(this);
}