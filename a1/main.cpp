#include <vector>
#include <iostream> 
using namespace std;

class SET {    //using stl vector allowed

public:
    vector<int> setArray = {}; //set object
    int size = setArray.size();

    SET() { //constructor
        setArray = {}; //create empty set
    }

    void print(){
        if(size>0) {
            cout << setArray[0] ;
            for(int i = 1; i<size ; i++) {
                cout << ","<< setArray[i] ;
            }
            cout << endl; //endl or \n?
        }         
    }


   void insertElt(int data) {   //O(n)
        if(size>0){
            int i = 0;
            while (i<size) { //O(n)
                if (setArray[i] > data) {
                    break;
                }
                i++;
            } // end up at elt just greater than  data
            if (setArray[i-1] != data) {  
                setArray.insert(setArray.begin() + i, data); //O(n)
                size++;
            }            
        } else {
            setArray.push_back(data); //o(1)
            size++;
        }   
        // if elt already not present search for the just bigger elt and insert at correct spot
    }

    void deleteElt(int data) { //assuming set exists and is
        int i = 0;
        while (i<size) { //O(n)
            if (setArray[i] > data) {
                break;
            }
            i++;
        } // end up at elt just greater than  data

        if(i<=size && i>0 && setArray[i-1] == data) { //if the elt being looked for is in the set, the no. just bigger will have index 1 to n-1 or i = n if it is the last elt
            setArray.erase(setArray.begin() + i -1); //O(n)
            size--;
        } //size print is handled in cmd2 below
    }

    bool contains(int data) { //will check whether empty or nonempty
        if (size > 0){ //nonempty
            int i = 0;
            while (i<size) { //O(n)
                if (setArray[i] > data) {
                    break;
                }
                i++;
            }  // end up at elt just greater than  data

            if(i<=size && i>0 && setArray[i-1] == data) { //if the elt being looked for is in the set, the no. just bigger will have index 1 to n-1 or i = n if it is the last elt
                return 1;
            } else {
                return 0; 
            }
        } else { //empty set
            return  0;
        }
    }

    void unionSET (SET set2) {
        if (set2.size > 0) {
            for (int x : set2.setArray) {
                if (!(contains(x))) { //look up 'this' pointer
                    insertElt(x);  //look for a way to suppress output of contains
                }
            }
        }        
    }

    void intersectionSET (SET set2) { //checkout with while loop
        if (size>0) { //result will be empty set if set1 is already empty
            if (set2.size > 0) {
                for (int i = 0; i < size; i++) {
                    int x = setArray[i];
                    if (!(set2.contains(x))) { //look up 'this' pointer
                        deleteElt(x); 
                        i--; //to make up for the lost index position 
                    }
                }
            } else { //set2 is empty
                for (int i = 0; i < size; i++) {
                    int x = setArray[i];
                        deleteElt(x); 
                        i--; //to make up for the lost index position 
                }
                // setArray.clear(); doesn't work. Why?
            } 
        }             
    }

    void differenceSET (SET set2) { //check out with while loop
        if (set2.size > 0) { //if set2 is nonempty
            for (int i = 0; i < size; i++) {
                int x = setArray[i];
                if (set2.contains(x)) { //look up 'this' pointer
                    deleteElt(x); 
                    i--; //to make up for the lost index position 
                }
            }
        }     
    }

    void symmDifferenceSET (SET set2) { //check out with while loop
        if (set2.size > 0) {
            for (int x : set2.setArray) {
                if (contains(x)) { //look up 'this' pointer
                    deleteElt(x); 
                } else {
                    insertElt(x);
                }
            }
        }       
    }
};

void newSET(vector<SET*> &v) {
    SET *s = new SET;
    v.push_back(s); //pushes a pointer into the vector
}

void cmd1(vector <SET*> &arrOfSETs, int &index, int setnum, int data) {
    if (index >= setnum) {
        if (index == setnum) {
            newSET(arrOfSETs); //create set if not there
            index++; //increase index
        }
        arrOfSETs[setnum]-> insertElt(data);
        cout<< arrOfSETs[setnum] -> size << endl;
    } else {
        cout << endl; 
    }
}

void cmd2(vector <SET*> &arrOfSETs, int &index, int setnum, int data) {
    if (index > setnum && (arrOfSETs[setnum] -> size) > 0) { //SET exists and is noempty
        if (arrOfSETs[setnum]->contains(data)) { //nonempty SET and data exist
            arrOfSETs[setnum]-> deleteElt(data);
        }
        cout<< arrOfSETs[setnum] -> size << endl;        //size to be printed even if data is not present
    } else if (arrOfSETs[setnum] -> size == 0){ //empty set
        cout<< arrOfSETs[setnum] -> size << endl; 
    } else { //SET DNE
        cout << -1 << endl; 
    }
}

void cmd3(vector <SET*> &arrOfSETs, int &index, int setnum, int data) {
    if (index > setnum) { //SET exists, returns 0 if empty
        bool output = arrOfSETs[setnum]->contains(data);
        cout << output << endl;
    } else { //SET DNE
        cout << -1 << endl; 
    }
}

void cmd4(vector <SET*> &arrOfSETs, int &index, int setnum, int data) {
    //here data = set_num2
    if (index >= setnum) {  
        if (((index >= setnum) && (index == data)) || ((index >= data) && (index == setnum))) {
            newSET(arrOfSETs); //create set2 if not there
            index++; //increase index
        } else if (((index == setnum) && (index + 1 == data)) || ((index == data) && (index + 1 == setnum))) {
            newSET(arrOfSETs); 
            newSET(arrOfSETs); //create set1 and set2 if not there
            index += 2; //increase index by 2
        }
        arrOfSETs[setnum]-> unionSET(*(arrOfSETs[data]));
        cout<< arrOfSETs[setnum] -> size << endl;
    } else {
        cout << endl; 
    }
}

void cmd5(vector <SET*> &arrOfSETs, int &index, int setnum, int data) {
    //here data = set_num2 
    if (index >= setnum) {  
        if (((index >= setnum) && (index == data)) || ((index >= data) && (index == setnum))) {
            newSET(arrOfSETs); //create set2 if not there
            index++; //increase index
        } else if (((index == setnum) && (index + 1 == data)) || ((index == data) && (index + 1 == setnum))) {
            newSET(arrOfSETs); 
            newSET(arrOfSETs); //create set1 and set2 if not there
            index += 2; //increase index by 2
        } 
        (arrOfSETs[setnum])-> intersectionSET(*(arrOfSETs[data])); //works only for nonempty sets
        cout<< ((arrOfSETs[setnum])-> size) << endl;
    } else {
        cout << endl; 
    }
}

void cmd6(vector <SET*> &arrOfSETs, int &index, int setnum) {
    if (index >= setnum) {
        if (index == setnum) {
            newSET(arrOfSETs); //create set if not there
            index++; //increase index
        }
        cout<< arrOfSETs[setnum] -> size << endl;
    } else {
        cout << endl; 
    }
}

void cmd7(vector <SET*> &arrOfSETs, int &index, int setnum, int data) {
    if (index >= setnum) {  
        if (((index >= setnum) && (index == data)) || ((index >= data) && (index == setnum))) {
            newSET(arrOfSETs); //create set2 if not there
            index++; //increase index
        } else if (((index == setnum) && (index + 1 == data)) || ((index == data) && (index + 1 == setnum))) {
            newSET(arrOfSETs); 
            newSET(arrOfSETs); //create set1 and set2 if not there
            index += 2; //increase index by 2
        } 
        arrOfSETs[setnum]-> differenceSET(*(arrOfSETs[data]));
        cout<< arrOfSETs[setnum] -> size << endl;
    } else {
        cout << endl; 
    }
}

void cmd8(vector <SET*> &arrOfSETs, int &index, int setnum, int data) {
    if (index >= setnum) {  
        if (((index >= setnum) && (index == data)) || ((index >= data) && (index == setnum))) {
            newSET(arrOfSETs); //create set2 if not there
            index++; //increase index
        } else if (((index == setnum) && (index + 1 == data)) || ((index == data) && (index + 1 == setnum))) {
            newSET(arrOfSETs); 
            newSET(arrOfSETs); //create set1 and set2 if not there
            index += 2; //increase index by 2
        } 
        arrOfSETs[setnum]-> symmDifferenceSET(*(arrOfSETs[data]));
        cout<< arrOfSETs[setnum] -> size << endl;
    } else {
        cout << endl; 
    }
}

void cmd9(vector <SET*> &arrOfSETs, int &index, int setnum) {
    if (index > setnum && (arrOfSETs[setnum]-> size) > 0) { //set exists
        arrOfSETs[setnum] -> print();
    } else { //setDNE or empty
        cout << endl; 
    }
}


int main()
{    
    vector <SET*> arrOfSETs = {};
    int index = 0; //stores index which is going to be filled next
    
    int n; 
    while (cin >> n) {        

        if ( n != 6 && n != 9) {
            int setnum, data;
            cin >> setnum >> data;
            switch (n) {           
                case 1: cmd1(arrOfSETs, index, setnum, data); break;
                case 2: cmd2(arrOfSETs, index, setnum, data); break;
                case 3: cmd3(arrOfSETs, index, setnum, data); break;
                case 4: cmd4(arrOfSETs, index, setnum, data); break;
                case 5: cmd5(arrOfSETs, index, setnum, data); break;
                case 7: cmd7(arrOfSETs, index, setnum, data); break;
                case 8: cmd8(arrOfSETs, index, setnum, data); break;
            }
        } else {
            int setnum;
            cin >> setnum;
            switch (n) {           
                case 6: cmd6(arrOfSETs, index, setnum); break;
                case 9: cmd9(arrOfSETs, index, setnum); break;
            }
        }
    }

    for (SET* p : arrOfSETs) {
        delete p;
    }

    return 0;    
}