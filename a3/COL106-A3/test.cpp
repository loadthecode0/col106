#include "Chaining.h"
#include "LinearProbing.h"
#include "QuadraticProbing.h"
#include "CubicProbing.h"
#include "Comp.h"
#include <cassert>

void testDatabase(BaseClass *db) {
    db->createAccount("SQRU2653821_2612357612", 1000);
    db->createAccount("DFVX2843917_2675751681", 1500);
    db->createAccount("SVTT2416826_8626716828", 2000);

    assert(db->getBalance("SQRU2653821_2612357612") == 1000);
    assert(db->getBalance("DFVX2843917_2675751681") == 1500);
    assert(db->getBalance("SVTT2416826_8626716828") == 2000);

    db->addTransaction("SQRU2653821_2612357612", -200);
    db->addTransaction("DFVX2843917_2675751681", 300);
    db->addTransaction("SVTT2416826_8626716828", -400);

    assert(db->getBalance("SQRU2653821_2612357612") == 800);
    assert(db->getBalance("DFVX2843917_2675751681") == 1800);
    assert(db->getBalance("SVTT2416826_8626716828") == 1600);

    std::vector<int> topBalances = db->getTopK(2);
    assert(topBalances.size() == 2);
    assert(topBalances[0] == 1800);
    assert(topBalances[1] == 1600);

    assert(db->databaseSize() == 3);

    assert(db->doesExist("SQRU2653821_2612357612"));
    assert(!db->doesExist("Eve"));
}


int main() {
    Chaining chainingDB;
    LinearProbing linearProbingDB;
    QuadraticProbing quadraticProbingDB;
    CubicProbing cubicProbingDB;
    // Comp compDB;

    std::cout << "Testing Chaining:" << std::endl;
    testDatabase(&chainingDB);

    std::cout << "\nTesting Linear Probing:" << std::endl;
    testDatabase(&linearProbingDB);

    std::cout << "\nTesting Quadratic Probing:" << std::endl;
    testDatabase(&quadraticProbingDB);

    std::cout << "\nTesting Cubic Probing:" << std::endl;
    testDatabase(&cubicProbingDB);

    // std::cout << "\nTesting Comp:" << std::endl;
    // testDatabase(&compDB);
    return 0;
}
