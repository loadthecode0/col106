#include "stack_b.h"
#include <iostream> // delete this?
#include <stdexcept>
//tbd: add throw statements (exception specifications) after each function

//stack top = array end;

Stack_B :: Stack_B () { //constructor
    try
    {
        stk = new int[1]; //create new array in heap memory
        capacity = 1; //physical capacity initialization
        size = 0; //logical size initialization
    }
    catch(std::bad_alloc)
    {
        throw std::runtime_error("Out of Memory");
    }    
}

 Stack_B :: ~Stack_B() { //destructor
    delete [] stk;
    stk = nullptr;
}

void Stack_B :: push(int data) {
    if (size < capacity) { //if current capacity is not fully occupied
        stk[size++] = data; //insert new elt and increase size
    } else { // if capacity is already fully occupied ie size = capacity
        //pre:cond : heap memory should be available
        capacity *= 2;
        try
        {
            int* temp = new int[capacity];            
            for (int i = 0; i< size; i++) {
                temp[i] = stk [i]; //fill temp array
            }
            delete stk; //free stk
            stk = temp; //reassign new array address to stk pointer
            stk[size++] = data;
            // post-cond : array expanded if memory available, new elt pushed
        }
        catch(std::bad_alloc)
        {
            throw std::runtime_error("Out of Memory");
        }
    }     
}

int Stack_B :: pop() {
    if (size>0) {
        //hysteresis application
        if (capacity >= 4096) { //shrink by 4 allowed
            if (size > 1 + (capacity/4)) { //no shrinking required
                return stk [--size]; //reduce size, returns popped elt
            } else { //size <= (actually =) 1 + cap/4 : shrink after pop. popped elt index = size-1
                //pre:cond : heap memory should be available
                int poppedElt = stk[size-1];
                try
                {
                    int* temp = new int[capacity/4];                
                    for (int i = 0; i< size-1; i++) {
                        temp[i] = stk [i]; //fill temp array
                    }
                    delete stk; //free stk 
                    stk = temp; //reassign new array address to stk pointer
                    size--; //actually now size = cap/4
                    return poppedElt;
                // post-cond : array shrunk if memory available, one elt popped, returned
                }
                catch(std::bad_alloc)
                {
                    throw std::runtime_error("Out of Memory");
                }
            }  
        }
        else if (capacity > 1024 && capacity < 4096) { //reduce shrink rate, ie don't shrink by four, shrink finally when size = 1024 only
            if (size > 1025) {
                return stk [--size]; 
            } else { //size = 1025 now, after pop, shrink to be 1024-sized
                int poppedElt = stk[size-1];
                try
                {
                    int* temp = new int[1024];                
                    for (int i = 0; i< size-1; i++) {
                        temp[i] = stk [i]; //fill temp array
                    }
                    delete stk; //free stk
                    stk = temp; //reassign new array address to stk pointer
                    // delete temp; //free temp
                    size--; //actually now size = cap/4
                    return poppedElt;
                    // post-cond : array shrunk if memory available, one elt popped, returned
                }
                catch(std::bad_alloc)
                {
                    throw std::runtime_error("Out of Memory");
                }
            }        
        } else { //capacity <= 1024 here
            return stk [--size];
        }  
    } else {
        throw std::runtime_error("Empty Stack");
    }         
}

int Stack_B :: get_element_from_top(int idx) {
    //precond : stack has at least (idx + 1) elts.
    if (size > idx) {
        return stk[size - 1 - idx];
    } else {
        throw std::runtime_error("Index out of range");
    }
    //post-cond : req elt is returned
};

int Stack_B :: get_element_from_bottom(int idx) {
    //precond : stack has at least (idx + 1) elts.
    if (size > idx) {
        return stk[idx];
    } else {
        throw std::runtime_error("Index out of range");
    }
    //post-cond : req elt is returned
};

void Stack_B :: print_stack(bool top_or_bottom) { 
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

int Stack_B :: add() {
    //precond : stack has at least 2 elts.
    if (size > 1) {
        int a = pop(); int b = pop(); //a is top, b is top-second
        push(b+a); //void return
        return (b+a);
    } else {
        throw std::runtime_error("Not Enough Arguments");
    }
    //post-cond : 2 pops and one push with req arithmetic done
}

int Stack_B :: subtract() {
    //precond : stack has at least 2 elts.
    if (size > 1) {
        int a = pop(); int b = pop(); //a is top, b is top-second
        push(b-a); //void return
        return (b-a);
    } else {
        throw std::runtime_error("Not Enough Arguments");
    }
    //post-cond : 2 pops and one push with req arithmetic done
}

int Stack_B :: multiply() {
    //precond : stack has at least 2 elts.
    if (size > 1) {
        int a = pop(); int b = pop(); //a is top, b is top-second
        push(b*a); //void return
        return (b*a);
    } else {
        throw std::runtime_error("Not Enough Arguments");
    }
    //post-cond : 2 pops and one push with req arithmetic done
}

int Stack_B :: divide() {
    //precond : stack has at least 2 elts.
    if (size > 1) {
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

int* Stack_B :: get_stack() { //confirm about this
    // int *ptr = stk;
    return stk;
}

int Stack_B :: get_size() { //confirm about this
    // int *ptr = stk;
    return size;
}