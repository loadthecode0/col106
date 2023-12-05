#include "stack_a.h"
#include <iostream> // delete this?
#include <stdexcept>

//stack top = array end;

Stack_A :: Stack_A () { //constructor
    size = 0; //size initialization
}

//inv : size = 1 + (index of last occupied element) 
//no destructor made; the array has been allocated in stack memory

void Stack_A :: push(int data) { //pushes data at the top of the stack
    //precond : empty space is there
    if (size < 1024) {
        stk[size] = data; //insert new elt
        size++; //increase size
    } else {
        throw std::runtime_error("Stack Full");
    }
    //post-cond: new data pushed, size increased by one
}

int Stack_A :: pop(){
    //precond : stack is non-empty
    if (size > 0) {
        return stk[--size]; //insert new elt
        // size--; //increase size
    } else {
        throw std::runtime_error("Empty Stack");
    }
    //post-cond: stk size decreased by one
}

int Stack_A :: get_element_from_top(int idx) {
    //precond : stack has at least (idx + 1) elts.
    if (size > idx) {
        return stk[size - 1 - idx];
    } else {
        throw std::runtime_error("Index out of range");
    }
    //post-cond : req elt is returned
};

int Stack_A :: get_element_from_bottom(int idx) {
    //precond : stack has at least (idx + 1) elts.
    if (size > idx) {
        return stk[idx];
    } else {
        throw std::runtime_error("Index out of range");
    }
    //post-cond : req elt is returned
};

void Stack_A :: print_stack(bool top_or_bottom) { 
    //precond : stack is not empty
    if (size > 0) {
        if (top_or_bottom == 0) { //print from bottom
            for (int i = 0; i<size; i++) {
                std :: cout << stk[i] << "\n";
            }
        } else {
            for (int i = 0; i<size; i++) { //print from top
                std :: cout << stk[size - 1 - i] << "\n";
            }
        }
    } 
    //post-cond: stk unaffected, printed elements
}

int Stack_A :: add() {
    //precond : stack has at least 2 elts.
    if (size > 1) {
        stk[size-2] = stk[size-2] + stk[size - 1];
        return stk[(size--) - 2];
    } else {
        throw std::runtime_error("Not Enough Arguments");
    }
    //post-cond : 2 pops and one push with req arithmetic done
};

int Stack_A :: subtract() {
    //precond : stack has at least 2 elts.
    if (size > 1) {
        stk[size-2] = stk[size-2] - stk[size - 1];
        return stk[(size--) - 2];
    } else {
        throw std::runtime_error("Not Enough Arguments");
    }
    //post-cond : 2 pops and one push with req arithmetic done
};

int Stack_A :: multiply() {
    //precond : stack has at least 2 elts.
    if (size > 1) {
        stk[size-2] = stk[size-2] * stk[size - 1];
        return stk[(size--) - 2];
    } else {
        throw std::runtime_error("Not Enough Arguments");
    }
    //post-cond : 2 pops and one push with req arithmetic done
};

int Stack_A :: divide() {
    //precond : stack has at least 2 elts.
    if (size > 1) {
        //precond : no division by zero => no zero at top
        if (stk[size-1] != 0) {
            if ((stk[size-1])*(stk[size-2]) < 0 && ((stk[size-1])%(stk[size-2]) != 0)) {
                stk[size-2] = (stk[size-2] / stk[size - 1]) - 1; //implicit typecast(?)
            } else {
                stk[size-2] = stk[size-2] / stk[size - 1]; //implicit typecast(?)
            }            
            return stk[(size--) - 2];
        } else {
            throw std::runtime_error("Divide by Zero Error");
        }
        //post cond: division done and quotient returned        
    } else {
        throw std::runtime_error("Not Enough Arguments");
    }
    //post-cond : 2 pops and one push with req arithmetic done
};

int* Stack_A :: get_stack() { //confirm about this
    // int *ptr = stk;
    return stk;
}

int Stack_A :: get_size() { //confirm about this
    // int *ptr = stk;
    return size;
}