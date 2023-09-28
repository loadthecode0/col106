/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "exprtreenode.h"

string ExprTreeNode:: type{ // Can be "ADD", "SUB", "MUL", "DIV", "MOD" or "VAL"
    
}
ExprTreeNode::UnlimitedRational* val; 

ExprTreeNode::UnlimitedRational* evaluated_value; 

ExprTreeNode::ExprTreeNode* left;
ExprTreeNode::ExprTreeNode* right;

ExprTreeNode::ExprTreeNode();

ExprTreeNode::ExprTreeNode(string t, UnlimitedInt* v);

ExprTreeNode::ExprTreeNode(string t, UnlimitedRational* v);

ExprTreeNode::~ExprTreeNode();