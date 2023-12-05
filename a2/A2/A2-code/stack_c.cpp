#include "list.h"
#include "node.h"
#include "stack_c.h"
#include <iostream>
#include <stdexcept>

Stack_C :: Stack_C(){
    try
    {
        stk = new List; //stk pointer points to list object
    }
    catch(std::bad_alloc)
    {
        throw std::runtime_error("Out of Memory");
    }   
}

Stack_C::~Stack_C(){
    stk->~List(); //the list object pointed to by stk is deleted
    delete stk; //the stk pointer is freed
    stk = nullptr;
}

void Stack_C::push(int data){
    stk->insert(data); //error handled by list's function
}

int Stack_C::pop() {
    if (stk->get_size() > 0) {
        return stk->delete_tail(); //error handled by list's function
    } else {
        throw std::runtime_error("Empty Stack");
    }    
}
    

int Stack_C :: get_element_from_top(int idx) { //top=tail!
//precond : stack has at least (idx + 1) elts.
    int t = stk->get_size();
    if (t > idx) {
        Node * traverser = stk->get_head(); //traverser is now pointing to the sentinel head
        while(t>idx){
            traverser = traverser -> next;
            t--;
        } //stops when t = idx, ie, size-idx = no. of iterations
        int output = traverser -> get_value();
        traverser = nullptr; //so that two pointers don't point to one object
        return output;
    } else {
        throw std::runtime_error("Index out of range");
    }    //post-cond : req elt is returned if present
}

int Stack_C :: get_element_from_bottom(int idx) {
    
    if (stk->get_size() > idx) {
        int t = 0;
        Node * traverser = stk->get_head(); //traverser is now pointing to the sentinel head
        while(t<=idx){
            traverser = traverser -> next;
            t++;
        } //stops when t = idx+1, ie, idx + 1 = no. of iterations
        int output = traverser -> get_value();
        traverser = nullptr; //so that two pointers don't point to one object
        return output;
    } else {
        throw std::runtime_error("Index out of range");
    }    //post-cond : req elt is returned if present
}

void Stack_C :: print_stack(bool top_or_bottom) {
    //precond : stack is not empty
    int t = stk->get_size();
    if (t > 0) {
        if (top_or_bottom == 0) { //print from bottom ie sent-head
            Node * traverser = stk->get_head()->next;  //first non-sentinel node
            while (t>0) {
                std :: cout << traverser->get_value() << "\n";
                traverser = traverser -> next;
                t--; //#iterations = size
            }
            traverser = nullptr;
        } else {
            Node * traverser = stk->get_head()->next;  //first non-sentinel node : this exists as size>0
            while (!(traverser->is_sentinel_node())) { //first find sent-tail
                traverser = traverser -> next;
            } //traverser is now sent-tail
            traverser = traverser -> prev; //traverser is now tail elt (not sent-tail)
            while (t>0) {
                std :: cout << traverser->get_value() << "\n";
                traverser = traverser -> prev;
                t--; //#iterations = size
            }
            traverser = nullptr;
        }
    }     //post-cond: stk unaffected, printed elements
}

int Stack_C::add() {
    //precond : stack has at least 2 elts.
    if (stk->get_size() > 1) {
        int a = pop(); int b = pop(); //a is top, b is top-second
        push(b+a); //void return
        return (b+a);
    } else {
        throw std::runtime_error("Not Enough Arguments");
    }   //post-cond : 2 pops and one push with req arithmetic done
}

int Stack_C :: subtract() {
    //precond : stack has at least 2 elts.
    if (stk->get_size() > 1) {
        int a = pop(); int b = pop(); //a is top, b is top-second
        push(b-a); //void return
        return (b-a);
    } else {
        throw std::runtime_error("Not Enough Arguments");
    }
    //post-cond : 2 pops and one push with req arithmetic done
}

int Stack_C :: multiply() {
    //precond : stack has at least 2 elts.
    if (stk->get_size() > 1) {
        int a = pop(); int b = pop(); //a is top, b is top-second
        push(b*a); //void return
        return (b*a);
    } else {
        throw std::runtime_error("Not Enough Arguments");
    }
    //post-cond : 2 pops and one push with req arithmetic done
}

int Stack_C :: divide() {
    //precond : stack has at least 2 elts.
    if (stk->get_size() > 1) {
        int a = pop(); int b = pop(); //a is top, b is top-second
        //precond : no division by zero => no zero at top
        if (a != 0) {
            if (a*b < 0 && (a%b != 0)) {
                push((b/a)-1);
                return((b/a)-1);
            } else {
                push(b/a); //void return
                return (b/a);
            } 
        } else { //a=0
            throw std::runtime_error("Divide by Zero Error");
        }
        //post cond: division done and quotient returned        
    } else {
        throw std::runtime_error("Not Enough Arguments");
    }
    //post-cond : 2 pops and one push with req arithmetic done
}

List* Stack_C :: get_stack() { //confirm about this
    // int *ptr = stk;
    return stk;
}

int Stack_C :: get_size() { //confirm about this
    // int *ptr = stk;
    return stk->get_size();
}