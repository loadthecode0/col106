#include <bits/stdc++.h>
using namespace std;

#define INT_BITS 8

// unsigned leftRotate(unsigned n, unsigned int d)
// {
    
//     /* In n<<d, last d bits are 0. To
//     put first 3 bits of n at
//     last, do bitwise or of n<<d
//     with n >>(INT_BITS - d) */
//     return (n << d)|(n >> (INT_BITS - d));
// }

// /*Function to right rotate n by d bits*/
// unsigned rightRotate(unsigned n, unsigned int d)
// {
//     /* In n>>d, first d bits are 0.
//     To put last 3 bits of at
//     first, do bitwise or of n>>d
//     with n <<(INT_BITS - d) */
//     return (n >> d)|(n << (INT_BITS - d));
// }


long long binary(int num) {



    long long bin = 0;
    int rem = 0;
    long long q = 0;
    int iterations = 0;
    long long multiplier = 1;
    while (num > 0) {
        rem = num % 2;
        bin = bin + rem * (multiplier);
        multiplier*= 10;
        num = num/2;
    }
    return bin;
}

uint32_t hashF(string id) {
    uint32_t c1 = 0xcc9e2d51;
    // printf("%d %x\n \n", c1, c1);
    uint32_t c2 = 0x1b873593;
    int r1 = 15;
    int r2 = 13;
    int m = 5;
    uint32_t n = 0xe6546b64;

    vector<uint32_t> block(6,0);
    uint32_t hash = 0;

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
    // printf("%x %d\n", hash, hash);
	hash ^= hash >> 16;
    hash = hash % 200033;
    // printf("%x %d\n", hash, hash);
    // cout << hash << "\n";
    return hash;
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
    

    vector<bool> occurred(200033, 0);
    srand(time(0));
    // cout << occurred.size();
    int collisions = 0;

    for (int i = 0; i < 100000; i++) {
        string* myId = new string;
        *myId = randomId();
        // cout << *myId << "\n";
        int hashCode = hashF(*myId);
        if (occurred[hashCode] == 0) {
            occurred[hashCode] = 1;
        } else {
            collisions++;
        }
        delete myId;
    }
    cout << collisions;

    return 0;
}