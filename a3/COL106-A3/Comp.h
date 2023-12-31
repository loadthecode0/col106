#ifndef COMP_H
#define COMP_H

#include "BaseClass.h"
#include <iostream>
#include <vector>

class Comp : public BaseClass {
public:
    Comp();
    void createAccount(std::string id, int count) override;
    std::vector<int> getTopK(int k) override;
    int getBalance(std::string id) override;
    void addTransaction(std::string id, int count) override;
    bool doesExist(std::string id) override;
    bool deleteAccount(std::string id) override;
    int databaseSize() override;
    int hash(std::string id) override;
    
private:
    
    // Other data members and functions specific to Your implementation
    int dbsize = 0;
    uint32_t hashF(std::string id, int seed);
    void swapElts(int* a, int* b);
    void quickSort(std::vector<int> &v, int start, int end);
};

#endif // COMP_H
