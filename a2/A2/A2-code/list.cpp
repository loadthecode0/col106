#include "list.h"
#include "node.h"
#include <iostream>
#include <stdexcept>

List::List() { //constructor
    size = 0; //empty initially
    try
    {
        sentinel_head = new Node(); //init
        sentinel_tail = new Node(); //init
        sentinel_head -> prev = nullptr;
        sentinel_head -> next = sentinel_tail;    
        sentinel_tail -> prev = sentinel_head;
        sentinel_tail -> next = nullptr;
    }
    catch(std::bad_alloc)
    {
        throw std::runtime_error("Out of Memory");
    }   
}

List :: ~List() { //destructor : delete all nodes
    if (sentinel_head->next == sentinel_tail) { //empty DLL
        delete sentinel_head; sentinel_head = nullptr;
        delete sentinel_tail; sentinel_tail = nullptr;
    } else { //non empty DLL
        if (sentinel_head->next != nullptr) { //for nonempty DLL, non-sentinel nodes are always initialized with value, can't be nullptr
            Node * temp = sentinel_head -> next;
            delete sentinel_head;
            sentinel_head = nullptr;
            while (!(temp->is_sentinel_node())) {
                temp = temp -> next; //catch next node
                delete temp -> prev; //delete contents of prev node
                temp -> prev = nullptr;  //reinit prev node
            } //loop ends at temp = sent-tail
            delete sentinel_tail;
        }
    }   
}

void List::insert(int v){
    try
    {
        Node * newNode = new Node(v, sentinel_tail, sentinel_tail->prev);  //new node initialised with value v and prev, next set to nullptr
        // newNode -> prev = sentinel_tail->prev;
        sentinel_tail->prev->next = newNode;
        // newNode->next = sentinel_tail;
        sentinel_tail->prev = newNode; //updated all connections
        size++;
    // post-cond : list expanded if memory available, new elt pushed
    }
    catch(std::bad_alloc)
    {
        throw std::runtime_error("Out of Memory");
    }  
}

int List ::  delete_tail(){
    //precond : LIST is non-empty
    if (size > 0) {
        Node * temp = sentinel_tail->prev;
        sentinel_tail->prev->prev->next = sentinel_tail;
        sentinel_tail -> prev = sentinel_tail -> prev -> prev;
        int output = temp->get_value();
        delete temp; //deleted the req node
        temp = nullptr;
        size--;
        return output;
    } else {
        throw std::runtime_error("Empty Stack");
    }
    //post-cond: LIST size decreased by one
    
}

int List :: get_size() {
    return size;
}

Node* List::get_head() {
    return sentinel_head;
}