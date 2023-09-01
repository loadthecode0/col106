#include "CubicProbing.h"

//note: for an account to be validly existing, it should have a nonempty id AND available[.] = 0

CubicProbing::CubicProbing() {
    bankStorage1d.resize(130003); //initialize vector of 130003 empty Accounts, each with id = "". (I checked it in scratch)
}

int CubicProbing::newKeyProbingResult (std::string &id) { //returns index after probing, hashCode is already modulo size
    int currPosition = hash(id);
    int iterations = 0;
    while (available[currPosition] == 0) { //occupied slot
        currPosition = (currPosition + iterations*iterations*iterations) % 130003;
        iterations++;
    } //probes till empty slot is found
    return currPosition;
}

void CubicProbing::createAccount(std::string id, int count) {
    int index = newKeyProbingResult(id);
    bankStorage1d[index] = Account {id, count};
    available[index] = 0;
    dbsize++;
}

void swapElts(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void quickSort(std::vector<int> &v, int start, int end) {
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

std::vector<int> CubicProbing::getTopK(int k) {
    std::vector<int> allBalances;
    std::vector<int> topK;

    //iterate over all accounts to push in allBalances;
    for (Account a : bankStorage1d) {
        if (a.balance > 0) {
            allBalances.push_back(a.balance);
        }
    }
    quickSort(allBalances, 0, allBalances.size() - 1); //sorts in ascending order
    int n = (k<=allBalances.size()) ? k : allBalances.size(); // n is min of k and dbsize
    for (int i = allBalances.size()-1; i > allBalances.size() - n - 1; i--) {
        topK.push_back(allBalances[i]);
    }
    return topK;
}

int CubicProbing::indexFinder (std::string &id) { //returns index at which key is present, -1 if not present.
    int currPosition = hash(id);    
    if (available[currPosition] == 0 && bankStorage1d[currPosition].id == id) { //filled and id matches
        return currPosition;
    } else if (available[currPosition] == 1) { //empty (marked)
        return -1;
    } else {
        int iterations = 0;
        while (available[currPosition] == 0 && bankStorage1d[currPosition].id != id) { //filled slots with wrong id
            currPosition = (currPosition + iterations*iterations*iterations) % 130003;
            iterations++;
            if (available[currPosition] == 0 && bankStorage1d[currPosition].id == id) { //filled and id matches
                return currPosition;
            }
            else if (available[currPosition] == 1) { //if not present, will definitely hit an empty spot while probing as table size is greater than max num of keys
                return -1;
            }
        }
    }
}

int CubicProbing::getBalance(std::string id) {
    int index = indexFinder(id);
    if (index != -1) { //acc exists
        return bankStorage1d[index].balance;
    } else {
        return -1;
    }
}

void CubicProbing::addTransaction(std::string id, int count) {
    int index = indexFinder(id);
    if (index != -1) { //acc exists
        bankStorage1d[index].balance += count;
    } else {
        createAccount(id, count);
    }
}

bool CubicProbing::doesExist(std::string id) {
    if (indexFinder (id) != -1) {
        return true;
    } else {
        return false;
    }
}

bool CubicProbing::deleteAccount(std::string id) {
    int index = indexFinder(id);
    if (index != -1) { //acc exists
        available[index] = 1; //make it available for reuse
        dbsize--;
        return true;
    } else {
        return false;
    }
}
int CubicProbing::databaseSize() {
    return dbsize;
}
int horner(const std::string& id, int x, int startInd, int endInd) {
    int output = int(id[endInd]); // Initialize output
    // Evaluate value of polynomial using Horner's method
    for (int i=endInd-1; i>=startInd; i--) {
        output = output*x + int(id[i]); //note: casting a numerical char to int gives a value shifted by 48
    } //polynomial this way so that later digits become more impactful
    return output;
}

int CubicProbing::hash(std::string id) {
    int a = horner (id, 3, 0, 3);
    int b = horner (id, 3, 4, 10); 
    int c = horner (id, 5, 12, 21);
    return (a+b+c) % 130003;
}