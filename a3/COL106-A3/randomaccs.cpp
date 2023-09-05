#include "LinearProbing.h"
#include <bits/stdc++.h>


using namespace std;

//note: for an account to be validly existing, it should have a nonempty id AND available[.] = 0

int collisions;

LinearProbing::LinearProbing() {
    bankStorage1d.resize(200023); //initialize vector of 200023 empty Accounts, each with id = "". (I checked it in scratch)
}

int LinearProbing::newKeyProbingResult (std::string &id) { //returns index after probing, hashCode is already modulo size
    int currPosition = hash(id);
    if (available[currPosition] == 0) {
        collisions++;
    }
    while (available[currPosition] == 0) { //occupied slot
        currPosition = (currPosition + 1) % 200023;
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
    for (int i = 0; i < 200023; i++) {
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
            currPosition = (currPosition + 1) % 200023; //probe and check
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

int LinearProbing::hash(string id) {
    uint32_t c1 = 0xcc9e2d51;
    // printf("%d %x\n \n", c1, c1);
    uint32_t c2 = 0x1b873593;
    int r1 = 15;
    int r2 = 13;
    int m = 5;
    uint32_t n = 0xe6546b64;

    vector<uint32_t> block(6,0);
    uint32_t hash = 0;

    block[0] = (int)id[0];
    // printf("%d %x %d\n", (int)id[0], block[0], block[0]);
    for (int i = 1; i < 4; i++) {
        block[0] = block[0] << 8;
        block[0] = block[0] + (int)id[i];        
        // printf("%d %x %d\n", (int)id[i], block[0], block[0]);        
    }

    // cout << "\n";

    block[1] = (int)id[4];
    // printf("%d %x %d\n", (int)id[4], block[1], block[1]);
    for (int i = 5; i < 8; i++) {
        block[1] = block[1] << 8;
        block[1] = block[1] + (int)id[i];        
        // printf("%d %x %d\n", (int)id[i], block[1], block[1]);       
    }

    // cout << "\n";

    block[2] = (int)id[8];
    // printf("%d %x %d\n", (int)id[8], block[2], block[2]);
    for (int i = 9; i < 11; i++) {
        block[2] = block[2] << 8;
        block[2] = block[2] + (int)id[i];    
        // printf("%d %x %d\n", (int)id[i], block[2], block[2]);    
    }

    // cout << "\n";

    block[3] = (int)id[12];
    // printf("%d %x %d\n", (int)id[12], block[3], block[3]);
    for (int i = 13; i < 16; i++) {
        block[3] = block[3] << 8;
        block[3] = block[3] + (int)id[i];
        // printf("%d %x %d\n", (int)id[i], block[3], block[3]);     
    }

    // cout << "\n";

    block[4] = (int)id[16];
    // printf("%d %x %d\n", (int)id[16], block[4], block[4]);
    for (int i = 17; i < 20; i++) {
        block[4] = block[4] << 8;
        block[4] = block[4] + (int)id[i];
        // printf("%d %x %d\n", (int)id[i], block[4], block[4]);        
    }

    // cout << "\n";

    block[5] = (int)id[20];
    // printf("%d %x %d\n", (int)id[20], block[5], block[5]);
    for (int i = 21; i < 22; i++) {
        block[5] = block[5] << 8;
        block[5] = block[5] + (int)id[i];
        // printf("%d %x %d\n", (int)id[i], block[5], block[5]);       
    }

    // cout << "\n";

    uint32_t s = block[0] * c1; //takes only most significant 32 bits of multiplication result.
    // printf("%x %d\n", s, s);
    // printf("%x \n", (s << r1));
    // printf("%x \n", (s >> (32 - r1)));
    
    s = (s << r1) | (s >> (32 - r1));
    // printf("%x %d\n", s, s);
    // cout << "\n";
    
    
    for (int i = 0; i < 6; i++) {
        uint64_t s = block[i]; 
        s *= c1;
        s = (s << r1) | (s >> (32 - r1));
        s *= c2; //fine till here.
        
        hash = hash ^ block[i];
        hash = (hash << r2) | (hash >> (32 - r2));
        hash = (hash * m) + n;
        // printf("%x %d\n", hash, hash);
    }
    // cout << "\n";
    

    //final mixing
    hash ^= 23; //ensures final bytes are well-incorporated?
    // printf("%x %d\n", hash, hash);
	hash ^= hash >> 16;
    // printf("%x %d\n", hash, hash);
	hash *= 0x85ebca6b;
    // printf("%x %d\n", hash, hash);
	hash ^= hash >> 13;
    // printf("%x %d\n", hash, hash);
	hash *= 0xc2b2ae35;
    // printf("%x %d\n", hash, hash);
	hash ^= hash >> 16;
    int finalHash = hash % 200033;
    // printf("%x %d\n", hash, hash);
    // cout << hash << "\n";
    return finalHash;
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
            int c = db->deleteAccount(v[rand()%200023]);
            if (c == 0) {
                k++;
            }
        } catch (runtime_error) {
            throw "exception";
        }
    }

    for (int i = 0; i<5000; i++) {
        try {
            int c = db->doesExist(v[rand()%200023]);
        } catch (runtime_error) {
            throw "exception";
        }
    }
    
    cout << k <<"\n";
    cout << db->databaseSize() << "\n";
    cout << collisions << "\n";

    

    return 0;
}