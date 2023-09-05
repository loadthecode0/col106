#include "LinearProbing.h"
#define hashPrime 200023

//note: for an account to be validly existing, it should have a nonempty id AND available[.] = 0

LinearProbing::LinearProbing() {
    bankStorage1d.resize(hashPrime); //initialize vector of hashPrime empty Accounts, each with id = "". (I checked it in scratch)
    available.resize(hashPrime, 1);
    dbsize = 0;
}

int LinearProbing::newKeyProbingResult (std::string &id) { //returns index after probing, hashCode is already modulo size
    int hashCode = hash(id);
    int currPosition = hashCode;
    int iterations = 0;
    while (available[currPosition] == 0) { //occupied slot
        int k = iterations % hashPrime;
        currPosition = (hashCode + k) % hashPrime;
        iterations++;
    } //probes till empty slot is found
    return currPosition;
}

void LinearProbing::createAccount(std::string id, int count) {
    int index = newKeyProbingResult(id);
    bankStorage1d[index] = Account {id, count};
    available[index] = 0;
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
    for (int i = 0; i < hashPrime; i++) {
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
    int hashCode = hash(id);
    long long currPosition = hashCode;  
    if (available[currPosition] == 0 && bankStorage1d[currPosition].id == id) { //filled and id matches
        return currPosition;
    } else if (available[currPosition] == 1) { //empty (marked)
        return -1;
    } else {
        int iterations = 0;
        while (available[currPosition] == 0 && bankStorage1d[currPosition].id != id) { //filled slots with wrong id
            int k = iterations;
            currPosition = (hashCode + k) % hashPrime;
            iterations++;
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
    int c = horner (id, 5, 12, 21);
    return (a+b+c) % hashPrime;
}

// int main() {
//     LinearProbing *db = new LinearProbing;

//     std::cout << db->databaseSize() << "\n";
//     db->createAccount("SBIN2390298_1212399209", 1000);
//     db->createAccount("SBIN827462_1212384829", 1500);
//     db->createAccount("SBIN2546182_5121789421", 2000);

//     std::cout << db->databaseSize() << "\n";

//     std::cout << db->getBalance("SBIN2390298_1212399209") << "\n";
//     std::cout << db->getBalance("SBIN827462_1212384829") << "\n";
//     std::cout << db->getBalance("SBIN2546182_5121789421")<< "\n";

//     std::cout << db->databaseSize() << "\n";

//     db->addTransaction("SBIN2390298_1212399209", -200);
//     db->addTransaction("SBIN827462_1212384829", 300);
//     db->addTransaction("SBIN2546182_5121789421", -400);

//     std::cout << db->databaseSize() << "\n";

//     // assert(db->getBalance("Alice") == 800);
//     // assert(db->getBalance("Bob") == 1800);
//     // assert(db->getBalance("Charlie") == 1600);
//     std::cout << db->getBalance("SBIN2390298_1212399209") << "\n";
//     std::cout << db->getBalance("SBIN827462_1212384829") << "\n";
//     std::cout << db->getBalance("SBIN2546182_5121789421")<< "\n";

//     std::vector<int> topBalances = db->getTopK(2);
//     for (int x : topBalances) {
//         std::cout << x << " ";
//     }
//     std :: cout << " \n";
//     std::cout << (topBalances.size() == 2) << "\n";
//     std :: cout << (topBalances[0] == 1800) << "\n";
//     std :: cout << (topBalances[1] == 1600) << "\n";

//     std :: cout << (db->databaseSize() == 3) << "\n";
//     std :: cout << (db->doesExist("SBIN2390298_1212399209")) << "\n";
//     std :: cout << (db->doesExist("SBIN2390298_1212399609")) << "\n";

//     // assert(db->databaseSize() == 3);

//     // assert(db->doesExist("Alice"));
//     // assert(!db->doesExist("Eve"));

//     std :: cout << (db->deleteAccount("SBIN2390298_1212399209")) << "\n";
//     std :: cout << (db->deleteAccount("SBIN2390298_1212399609")) << "\n";
//     std::cout << db->databaseSize() << "\n";
// }