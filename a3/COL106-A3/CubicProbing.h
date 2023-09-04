#ifndef CUBICPROBING_H
#define CUBICPROBING_H

#include "BaseClass.h"
#include <iostream>

class CubicProbing : public BaseClass {
public:
    CubicProbing();
    void createAccount(std::string id, int count) override;
    std::vector<int> getTopK(int k) override;
    int getBalance(std::string id) override;
    void addTransaction(std::string id, int count) override;
    bool doesExist(std::string id) override;
    bool deleteAccount(std::string id) override;
    int databaseSize() override;
    int hash(std::string id) override;
    
private:
    // Other data members and functions specific to Quadratic Probing
    int dbsize = 0;
    std::vector<int> available = std::vector<int>(130003, 1); //for finding if slot is empty or not; modified after additions and deletions.
    int newKeyProbingResult (std::string &id);
    int indexFinder (std::string &id);
    int horner(const std::string& id, int x, int startInd, int endInd);
    void swapElts(int* a, int* b);
    void quickSort(std::vector<int> &v, int start, int end);
};

#endif // CUBICPROBING_H
