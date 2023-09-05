#include <bits/stdc++.h>
using namespace std;

#define hashPrime 200023

#define INT_BITS 8

int horner(const std::string& id, int x, int startInd, int endInd) {
    int output = int(id[endInd]); // Initialize output
    // Evaluate value of polynomial using Horner's method
    for (int i=endInd-1; i>=startInd; i--) {
        output = output*x + int(id[i]); //note: casting a numerical char to int gives a value shifted by 48
    } //polynomial this way so that later digits become more impactful
    return output;
}

int hash1(std::string id) {
    int a = horner (id, 3, 0, 3);
    int b = horner (id, 3, 4, 10); 
    int c = horner (id, 5, 12, 21);
    return (a+b+c) % hashPrime;
}

uint32_t hashF(string id, int seed) {
    uint32_t c1 = 0xcc9e2d51;
    // printf("%d %x\n \n", c1, c1);
    uint32_t c2 = 0x1b873593;
    int r1 = 15;
    int r2 = 13;
    int m = 5;
    uint32_t n = 0xe6546b64;

    vector<uint32_t> block(6,0);
    uint32_t hash = (uint32_t)seed;

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
    printf("%x %d\n", hash, hash);
	hash ^= hash >> 16;
    int finalHash = hash % 200033;
    // printf("%x %d\n", hash, hash);
    // cout << hash << "\n";
    return finalHash;
}

string randomId() {
    // srand(time(0));
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

int main()
{
    

    vector<bool> occurred(hashPrime, 0);
    vector<bool> visited(hashPrime, 0);
    srand(time(0));
    // cout << occurred.size();
    int collisions = 0;
    int nZero = 0;
    int nVisited = 0;

    for (int i = 0; i < 100000; i++) {
        string* myId = new string;
        *myId = randomId();
        // cout << *myId << "\n";
        int hashCode = hashF(*myId, hash1(*myId));
        if (hashCode == 0) {nZero++;};
        if (occurred[hashCode] == 0) {
            occurred[hashCode] = 1;
        } else {
            collisions++;
        }
        
        delete myId;
    }
    cout << nZero << "\n";
    cout << collisions;

    return 0;
}