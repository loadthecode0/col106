#include "Chaining.h"

Chaining :: Chaining () { //constructor
    bankStorage2d.resize(130003); //initialize empty vector
}

bool Chaining ::bucketExists (int hashCode) {
    return bankStorage2d[hashCode].size(); //returns 0 if empty, 1 if nonempty
    //note: bucket vector is created only when an account with that hash code is created.
}

void Chaining::createAccount(std::string id, int count) {
    //note: bucket vector is created only when an account with that hash code is created.
    Account newAcc {id, count}; //instantiate account struct 
    int accHash = hash(id); //generate hash code
    if (!bucketExists(accHash)) { //brand new hash code generated
        std::vector<Account> newBucket = {newAcc}; //create acc list (bucket) for this and push new acc into bucket
        bankStorage2d[accHash] = newBucket; //push 
    } else { //collision situation
        bankStorage2d[accHash].push_back(newAcc); //push new acc into existing bucket at that hash value
    }  
    dbsize++;  
}

std::vector<int> Chaining::getTopK(int k) {
    // IMPLEMENT YOUR CODE HERE
    return std::vector<int>(); // Placeholder return value
}

int Chaining::getBalance(std::string id) {
    int accHash = hash(id); //generate hash code
    if (!bucketExists(accHash)) {
        return -1;
    } else { //collision situation
        for (Account a:bankStorage2d[accHash]) { //iterate over all accounts in bucket
            if (a.id == id) {
                return a.balance;
            }
        }
        return -1; //not found
    }
}

void Chaining::addTransaction(std::string id, int count) {
    int accHash = hash(id); //generate hash code
    if (!bucketExists(accHash)) {
        createAccount(id, count);
    } else { //collision situation
        for (Account a:bankStorage2d[accHash]) { //iterate over all accounts in bucket
            if (a.id == id) {
                a.balance += count;
                break;
            } 
        }
        createAccount(id, count); //acc not found, won't come here if break is used
    }
}

bool Chaining::doesExist(std::string id) {
    int accHash = hash(id); //generate hash code
    if (!bucketExists(accHash)) {
        return false;
    } else { //collision situation
        for (Account a:bankStorage2d[accHash]) { //iterate over all accounts in bucket
            if (a.id == id) {
                return true;
            }
        }
        return false; //not found
    }
}

bool Chaining::deleteAccount(std::string id) {
    int accHash = hash(id); //generate hash code
    if (!bucketExists(accHash)) {
        return false;
    } else { //collision situation
        int i = 0;
        while (i<bankStorage2d[accHash].size()) {
            if ((bankStorage2d[accHash])[i].id == id) {
                (bankStorage2d[accHash]).erase(bankStorage2d[accHash].begin() + i);
                return true; break; //break wasn't necessary ig
            }
            i++;
        }
        return false; //not found
    }
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
    return (a+b+c) % 1300003;
}

