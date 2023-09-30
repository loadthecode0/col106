/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "exprtreenode.h"

ExprTreeNode::ExprTreeNode() {
    type = "newNode"; //dummy value
    val = nullptr;
    evaluated_value =nullptr;
    left = nullptr;
    right = nullptr;
}

ExprTreeNode::ExprTreeNode(string t, UnlimitedInt* v) {
    type = t;
    val = new UnlimitedRational(v, new UnlimitedInt("1"));
    evaluated_value = new UnlimitedRational(new UnlimitedInt("0"), new UnlimitedInt("1"));
    left = nullptr;
    right = nullptr;
}

ExprTreeNode::ExprTreeNode(string t, UnlimitedRational* v) {
    type = t;
    val = v;
    evaluated_value = new UnlimitedRational(new UnlimitedInt("0"), new UnlimitedInt("1"));
    left = nullptr;
    right = nullptr;
}

ExprTreeNode::~ExprTreeNode() {

}