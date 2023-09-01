#ifndef LINEARPROBING_H
#define LINEARPROBING_H

#include "BaseClass.h"
#include <iostream>

class LinearProbing : public BaseClass {
public:
    LinearProbing();
    void createAccount(std::string id, int count) override;
    std::vector<int> getTopK(int k) override;
    int getBalance(std::string id) override;
    void addTransaction(std::string id, int count) override;
    bool doesExist(std::string id) override;
    bool deleteAccount(std::string id) override;
    int databaseSize() override;
    int hash(std::string id) override;
    
private:
    // Other data members and functions specific to Linear Probing
    int dbsize = 0;
    std::vector<int> available = std::vector<int>(130003, 1); //for finding if slot is empty or not; modified after additions and deletions.
    int newKeyProbingResult (std::string &id);
    int indexFinder (std::string &id);
};

#endif // LINEARPROBING_H
