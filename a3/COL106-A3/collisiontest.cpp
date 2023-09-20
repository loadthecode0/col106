#include <bits/stdc++.h>
using namespace std;

int horner(const std::string& id, int x, int startInd, int endInd) {
    int output = int(id[endInd]); // Initialize output
    // Evaluate value of polynomial using Horner's method
    for (int i=endInd-1; i>=startInd; i--) {
        output = (output % 200023)*x + int(id[i]); //note: casting a numerical char to int gives a value shifted by 48
        //did 200023 to prevent overflow
    } //polynomial this way so that later digits become more impactful
    return output;
}

int hashF(std::string id, int seed) {
    uint32_t prime1 = 0x9e3779b1; //golden ratio constant
    uint32_t prime2 = 0x517CC1B7; //2**32 / pi integer part
    int offset1 = 15;
    int offset2 = 13;
    int prime3 = 5;
    uint32_t prime4 = 0x243430A3; //2**32 * sqrt(2) integer part

    std::vector<uint32_t> block(6,0);
    uint32_t hash = (uint32_t)seed;

    block[0] = (int)id[0];
    for (int i = 1; i < 4; i++) {
        block[0] = block[0] << 8;
        block[0] = block[0] + (int)id[i];              
    }

    block[1] = (int)id[4];
    for (int i = 5; i < 8; i++) {
        block[1] = block[1] << 8;
        block[1] = block[1] + (int)id[i];             
    }

    block[2] = (int)id[8];
    for (int i = 9; i < 11; i++) {
        block[2] = block[2] << 8;
        block[2] = block[2] + (int)id[i];       
    }

    block[3] = (int)id[12];
    for (int i = 13; i < 16; i++) {
        block[3] = block[3] << 8;
        block[3] = block[3] + (int)id[i];   
    }

    block[4] = (int)id[16];
    for (int i = 17; i < 20; i++) {
        block[4] = block[4] << 8;
        block[4] = block[4] + (int)id[i];      
    }

    block[5] = (int)id[20];
    for (int i = 21; i < 22; i++) {
        block[5] = block[5] << 8;
        block[5] = block[5] + (int)id[i];     
    }
    
    for (int i = 0; i < 6; i++) {
        uint32_t s = block[i]; //takes only most significant 32 bits of multiplication result.
    
        s *= prime1;
        s = (s << offset1) | (s >> (32 - offset1)); //left rotate
        s *= prime2;
        
        hash = hash ^ s; //xor with previous value
        hash = (hash << offset2) | (hash >> (32 - offset2)); //left rotate
        hash = (hash * prime3) + prime4;
    }   

    //final mixing of bits
    hash ^= 23; //ensures final bytes are well-incorporated?
	hash ^= hash >> 16;
	hash *= 0x1A2C2885; //2**32 * sqrt(3) integer part till 32 bits
	hash ^= hash >> 13;
	hash *= 0x5144113A; //2**32 * sqrt(7) integer part till 32 bits
	hash ^= hash >> 16;

    return hash%202109;
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
    

    vector<bool> occurred(202109, 0);
    srand(time(0));
    // cout << occurred.size();
    int collisions = 0;

    for (int i = 0; i < 100000; i++) {
        string* myId = new string;
        *myId = randomId();
        // cout << *myId << "\n";
        int hashCode = hashF(*myId, 202109);
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

//current: about v close to 21% for 1L inputs