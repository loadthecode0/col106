#include "LinearProbing.h"
#include <bits/stdc++.h>


using namespace std;

//note: for an account to be validly existing, it should have a nonempty id AND available[.] = 0

int collisions;

LinearProbing::LinearProbing() {
    bankStorage1d.resize(130003); //initialize vector of 130003 empty Accounts, each with id = "". (I checked it in scratch)
}

int LinearProbing::newKeyProbingResult (std::string &id) { //returns index after probing, hashCode is already modulo size
    int currPosition = hash(id);
    if (available[currPosition] == 0) {
        collisions++;
    }
    while (available[currPosition] == 0) { //occupied slot
        currPosition = (currPosition + 1) % 130003;
    }
    return currPosition;
}

void LinearProbing::createAccount(std::string id, int count) {
    int index = newKeyProbingResult(id);
    bankStorage1d[index] = Account {id, count};
    available[index] = 0; //mark as occupied
    dbsize++;
}

void LinearProbing::swapElts(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void LinearProbing::quickSort(std::vector<int> &v, int start, int end) {
    if (start < end) {
        int n = end - start + 1;
        int pivotIndex = ((rand()) % (end - start + 1)) + start;
        int pivot = v[pivotIndex];

        //swap pivot with last
        swapElts(&v[pivotIndex], &v[end]);

        int i = start;
        while (i<end && v[i] <= pivot) {
            i++;
        } //ends up at first element greater than pivot elt

        if (i == end) { //all elts other than pivot are less than it 
            quickSort(v, start, end-1);
        } else {
            for (int j = i+1; j<=end; j++) {
                if (v[j] <= pivot){
                    swapElts(&v[i], &v[j]);
                    i++; //now filled by smaller elements till index i-1
                }
            }

            //swap i+1 and pivot(last)
            swapElts(&v[i], &v[end]);
            //called recursion for partition
            quickSort(v, start, i-2);
            quickSort(v, i, end);

        }
    }  
}

std::vector<int> LinearProbing::getTopK(int k) {
    std::vector<int> allBalances;
    std::vector<int> topK;

    //iterate over all accounts to push in allBalances;
    for (int i = 0; i < 130003; i++) {
        if (available[i] == 0) { //FILLED slots
            allBalances.push_back(bankStorage1d[i].balance);
        }
    }
    quickSort(allBalances, 0, dbsize - 1); //sorts in ascending order
    int n = (k<=dbsize) ? k : dbsize; // n is min of k and dbsize
    for (int i = dbsize-1; i > dbsize - n - 1; i--) {
        topK.push_back(allBalances[i]);
    }

    return topK;
}

int LinearProbing::indexFinder (std::string &id) { //returns index at which key is present, -1 if not present.
    int currPosition = hash(id);
    if (available[currPosition] == 0 && bankStorage1d[currPosition].id == id) { //filled and id matches
        return currPosition;
    } else if (available[currPosition] == 1) { //empty (marked)
        return -1;
    } else {
        while (available[currPosition] == 0 && bankStorage1d[currPosition].id != id) { //filled slots with wrong id
            currPosition = (currPosition + 1) % 130003; //probe and check
            if (available[currPosition] == 0 && bankStorage1d[currPosition].id == id) { //filled and id matches
                return currPosition;
            }
            else if (available[currPosition] == 1) { //if not present, will definitely hit an empty spot while probing as table size is greater than max num of keys
                return -1;
            }
        }
    }
    return -1; //default return value
}

int LinearProbing::getBalance(std::string id) {
    int index = indexFinder(id);
    if (index != -1) { //acc exists
        return bankStorage1d[index].balance;
    } else {
        return -1;
    }
    return -1;  //default return value
}

void LinearProbing::addTransaction(std::string id, int count) {
    int index = indexFinder(id);
    if (index != -1) { //acc exists
        bankStorage1d[index].balance += count;
    } else {
        createAccount(id, count);
    }
}

bool LinearProbing::doesExist(std::string id) {
    if (indexFinder (id) != -1) {
        return true;
    } else {
        return false;
    }
}

bool LinearProbing::deleteAccount(std::string id) {
    int index = indexFinder(id);
    if (index != -1) { //acc exists
        available[index] = 1; //make it available for reuse
        dbsize--;
        return true;
    } else {
        return false;
    }
}
int LinearProbing::databaseSize() {
    return dbsize;
}
int LinearProbing::horner(const std::string& id, int x, int startInd, int endInd) {
    int output = int(id[endInd]); // Initialize output
    // Evaluate value of polynomial using Horner's method
    for (int i=endInd-1; i>=startInd; i--) {
        output = output*x + int(id[i]); //note: casting a numerical char to int gives a value shifted by 48
    } //polynomial this way so that later digits become more impactful
    return output;
}

int LinearProbing::hash(std::string id) {
    int a = horner (id, 3, 0, 3);
    int b = horner (id, 3, 4, 10); 
    int c = horner (id, 5 , 12, 21);
    return (a+b+c) % 130003;
}

string randomId() {
    string id = "";

    for (int i =0; i<4; i++) {
        int n = 65 + (rand() % 26);
        id = id + char(n);
    }
    for (int i =0; i<7; i++) {
        int n = 48 + (rand() % 10);
        id = id + char(n);
    }
    id = id + '_';
    for (int i =0; i<10; i++) {
        int n = 48 + (rand() % 10);
        id = id + char(n);
    }
    return id;
}

int main() {

    LinearProbing *db = new LinearProbing;

    srand(time(0));
    vector <string> v(100000);
    for (int i = 0; i<100000; i++) {
        v[i] = randomId();
    }

    for (int i = 0; i<100000; i++) {
        try {
            db->createAccount(v[i], 0);
            // cout << db->databaseSize() << "\n";
        } catch (runtime_error) {
            throw "exception";
        }
        
    }

    for (int i = 0; i<100000; i++) {
        try {
            // cout << db->doesExist(v[i]) << "\n";
        } catch (runtime_error) {
            throw "exception";
        }        
    }

    int k = 0;
    for (int i = 0; i<5000; i++) {
        try {
            int c = db->deleteAccount(v[rand()%130003]);
            if (c == 0) {
                k++;
            }
        } catch (runtime_error) {
            throw "exception";
        }
    }

    for (int i = 0; i<5000; i++) {
        try {
            int c = db->doesExist(v[rand()%130003]);
        } catch (runtime_error) {
            throw "exception";
        }
    }
    
    cout << k <<"\n";
    cout << db->databaseSize() << "\n";
    cout << collisions << "\n";

    

    return 0;
}