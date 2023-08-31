#include "Chaining.h"

Chaining :: Chaining () { //constructor
    bankStorage2d.resize(130003, {}); //initialize vector of 130003 empty buckets
}

void Chaining::createAccount(std::string id, int count) {
    //note: bucket vector is created during DB initialization.
    Account newAcc {id, count}; //instantiate account struct 
    int accHash = hash(id); //generate hash code
    bankStorage2d[accHash].push_back(newAcc); 
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

std::vector<int> Chaining::getTopK(int k) {
    std::vector<int> allBalances;
    std::vector<int> topK;

    //iterate over all accounts to push in allBalances;
    for (int i = 0; i < 130003; i++) {
        for (Account a : bankStorage2d[i]) {
            allBalances.push_back(a.balance);
        }
    }
    quickSort(allBalances, 0, dbsize-1); //sorts in ascending order
    int n = (k<=dbsize) ? k : dbsize; // n is min of k and dbsize
    for (int i = dbsize-1; i > 0; i--) {
        topK.push_back(allBalances[i]);
    }

    return topK;
}

int Chaining::getBalance(std::string id) {
    int accHash = hash(id); //generate hash code
    for (Account a:bankStorage2d[accHash]) { //iterate over all accounts in bucket
        if (a.id == id) {
            return a.balance;
        }
    }
    return -1; //not found
}

void Chaining::addTransaction(std::string id, int count) {
    int accHash = hash(id); //generate hash code
    for (int i = 0; i < bankStorage2d[accHash].size(); i++) { //iterate over all accounts in bucket
        if (bankStorage2d[accHash][i].id == id) {
            bankStorage2d[accHash][i].balance += count;
            return;
        } 
    } 
    createAccount(id, count); //acc not found, won't come here if break is used
}

bool Chaining::doesExist(std::string id) {
    int accHash = hash(id); //generate hash code
    for (Account a:bankStorage2d[accHash]) { //iterate over all accounts in bucket
        if (a.id == id) {
            return true;
        }
    }
    return false; //not found
}

bool Chaining::deleteAccount(std::string id) {
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
int Chaining::databaseSize() {
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

int Chaining::hash(std::string id) {
    int a = horner (id, 3, 0, 3);
    int b = horner (id, 3, 4, 10); 
    int c = horner (id, 5, 12, 21);
    return (a+b+c) % 130003;
}

int main() {
    Chaining *db = new Chaining;

    std::cout << db->databaseSize() << "\n";
    db->createAccount("SBIN2390298_1212399209", 1000);
    db->createAccount("SBIN827462_1212384829", 1500);
    db->createAccount("SBIN2546182_5121789421", 2000);

    std::cout << db->databaseSize() << "\n";

    std::cout << db->getBalance("SBIN2390298_1212399209") << "\n";
    std::cout << db->getBalance("SBIN827462_1212384829") << "\n";
    std::cout << db->getBalance("SBIN2546182_5121789421")<< "\n";

    std::cout << db->databaseSize() << "\n";

    db->addTransaction("SBIN2390298_1212399209", -200);
    db->addTransaction("SBIN827462_1212384829", 300);
    db->addTransaction("SBIN2546182_5121789421", -400);

    std::cout << db->databaseSize() << "\n";

    // assert(db->getBalance("Alice") == 800);
    // assert(db->getBalance("Bob") == 1800);
    // assert(db->getBalance("Charlie") == 1600);
    std::cout << db->getBalance("SBIN2390298_1212399209") << "\n";
    std::cout << db->getBalance("SBIN827462_1212384829") << "\n";
    std::cout << db->getBalance("SBIN2546182_5121789421")<< "\n";

    std::vector<int> topBalances = db->getTopK(2);
    for (int x : topBalances) {
        std::cout << x << " ";
    }
    std :: cout << " \n";
    std::cout << (topBalances.size() == 2) << "\n";
    std :: cout << (topBalances[0] == 1800) << "\n";
    std :: cout << (topBalances[1] == 1600) << "\n";

    std :: cout << (db->databaseSize() == 3) << "\n";
    std :: cout << (db->doesExist("SBIN2390298_1212399209")) << "\n";
    std :: cout << (db->doesExist("SBIN2390298_1212399609")) << "\n";

    // assert(db->databaseSize() == 3);

    // assert(db->doesExist("Alice"));
    // assert(!db->doesExist("Eve"));

    std :: cout << (db->deleteAccount("SBIN2390298_1212399209")) << "\n";
    std :: cout << (db->deleteAccount("SBIN2390298_1212399609")) << "\n";
    std::cout << db->databaseSize() << "\n";
}