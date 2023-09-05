#include "Comp.h"
#define hashPrime 130003

Comp :: Comp () { //constructor
    bankStorage2d.resize(hashPrime, {}); //initialize vector of hashPrime empty buckets
}

void Comp::createAccount(std::string id, int count) {
    //note: bucket vector is created during DB initialization.
    Account newAcc {id, count}; //instantiate account struct 
    int accHash = hash(id); //generate hash code
    bankStorage2d[accHash].push_back(newAcc); 
    dbsize++;
}

void Comp::swapElts(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void Comp::quickSort(std::vector<int> &v, int start, int end) {
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

std::vector<int> Comp::getTopK(int k) {
    std::vector<int> allBalances;
    std::vector<int> topK;

    //iterate over all accounts to push in allBalances;
    for (int i = 0; i < hashPrime; i++) {
        for (Account a : bankStorage2d[i]) {
            allBalances.push_back(a.balance);
        }
    }
    quickSort(allBalances, 0, dbsize-1); //sorts in ascending order
    int n = (k<=dbsize) ? k : dbsize; // n is min of k and dbsize
    for (int i = dbsize-1; i > dbsize-n-1; i--) {
        topK.push_back(allBalances[i]);
    }

    return topK;
}

int Comp::getBalance(std::string id) {
    int accHash = hash(id); //generate hash code
    for (Account a:bankStorage2d[accHash]) { //iterate over all accounts in bucket
        if (a.id == id) {
            return a.balance;
        }
    }
    return -1; //not found
}

void Comp::addTransaction(std::string id, int count) {
    int accHash = hash(id); //generate hash code
    for (int i = 0; i < bankStorage2d[accHash].size(); i++) { //iterate over all accounts in bucket
        if (bankStorage2d[accHash][i].id == id) {
            bankStorage2d[accHash][i].balance += count;
            return;
        } 
    } 
    createAccount(id, count); //acc not found, won't come here if break is used
}

bool Comp::doesExist(std::string id) {
    int accHash = hash(id); //generate hash code
    for (Account a:bankStorage2d[accHash]) { //iterate over all accounts in bucket
        if (a.id == id) {
            return true;
        }
    }
    return false; //not found
}

bool Comp::deleteAccount(std::string id) {
    int accHash = hash(id); //generate hash code
    int i = 0;
    while (i<bankStorage2d[accHash].size()) {
        if ((bankStorage2d[accHash])[i].id == id) {
            (bankStorage2d[accHash]).erase(bankStorage2d[accHash].begin() + i);
            dbsize--;   
            return true; break; //break wasn't necessary ig
        }
        i++;
    }
    return false; //not found
}
int Comp::databaseSize() {
    return dbsize;
}
int Comp::hash(std::string id) {
    uint32_t hashCode = hashF(id, hashPrime);
    int finalHash = hashCode % hashPrime;
    return finalHash; 
}

uint32_t Comp::hashF(std::string id, int seed) {
    uint32_t c1 = 0xcc9e2d51;
    uint32_t c2 = 0x1b873593;
    int r1 = 15;
    int r2 = 13;
    int m = 5;
    uint32_t n = 0xe6546b64;

    std::vector<uint32_t> block(6,0);
    uint32_t hash = (uint32_t)seed;

    block[0] = (int)id[0];
    for (int i = 1; i < 4; i++) {
        block[0] = block[0] << 8;
        block[0] = block[0] + (int)id[i];              
    }

    block[1] = (int)id[4];
    for (int i = 5; i < 8; i++) {
        block[1] = block[1] << 8;
        block[1] = block[1] + (int)id[i];             
    }

    block[2] = (int)id[8];
    for (int i = 9; i < 11; i++) {
        block[2] = block[2] << 8;
        block[2] = block[2] + (int)id[i];       
    }

    block[3] = (int)id[12];
    for (int i = 13; i < 16; i++) {
        block[3] = block[3] << 8;
        block[3] = block[3] + (int)id[i];   
    }

    block[4] = (int)id[16];
    for (int i = 17; i < 20; i++) {
        block[4] = block[4] << 8;
        block[4] = block[4] + (int)id[i];      
    }

    block[5] = (int)id[20];
    for (int i = 21; i < 22; i++) {
        block[5] = block[5] << 8;
        block[5] = block[5] + (int)id[i];     
    }

    uint32_t s = block[0] * c1; //takes only most significant 32 bits of multiplication result.
    
    s = (s << r1) | (s >> (32 - r1)); //left rotate
    
    for (int i = 0; i < 6; i++) {
        uint64_t s = block[i]; 
        s *= c1;
        s = (s << r1) | (s >> (32 - r1));
        
        hash = hash ^ block[i];
        hash = (hash << r2) | (hash >> (32 - r2)); //left rotate
        hash = (hash * m) + n;
    }   

    //final mixing
    hash ^= 23; //ensures final bytes are well-incorporated?
	hash ^= hash >> 16;
	hash *= 0x85ebca6b;
	hash ^= hash >> 13;
	hash *= 0xc2b2ae35;
	hash ^= hash >> 16;

    return hash;
}
