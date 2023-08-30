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
}

std::vector<int> Chaining::getTopK(int k) {
    // IMPLEMENT YOUR CODE HERE
    return std::vector<int>(); // Placeholder return value
}

int Chaining::getBalance(std::string id) {
    int accHash = hash(id); //generate hash code
    if (!bucketExists(accHash)) { //brand new hash code generated
        return -1;
    } else { //collision situation
        for () //push new acc into existing bucket at that hash value
    }
    return 0; // Placeholder return value
}

void Chaining::addTransaction(std::string id, int count) {
    // IMPLEMENT YOUR CODE HERE
}

bool Chaining::doesExist(std::string id) {
    // IMPLEMENT YOUR CODE HERE
    return false; // Placeholder return value
}

bool Chaining::deleteAccount(std::string id) {
    // IMPLEMENT YOUR CODE HERE
    return false; // Placeholder return value
}
int Chaining::databaseSize() {
    // IMPLEMENT YOUR CODE HERE
    return 0; // Placeholder return value
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

