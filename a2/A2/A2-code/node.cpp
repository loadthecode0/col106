#include "node.h"
#include <iostream>
#include <stdexcept>

Node :: Node(bool sentinel) {
    // value = NULL;
    is_sentinel = true;
    prev = nullptr; //initial
    next = nullptr; //initial
}

Node ::  Node(int v, Node* nxt, Node* prv) {
    value = v;
    is_sentinel = false;
    prev = prv;
    next = nxt;
}

bool Node::is_sentinel_node() {
    return is_sentinel;
}

int Node :: get_value() {
    return value;
}