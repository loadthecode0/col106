#include <bits/stdc++.h>
using namespace std;

int horner(const std::string& id, int x, int startInd, int endInd) {
    int output = int(id[endInd]); // Initialize output
    // Evaluate value of polynomial using Horner's method
    for (int i=endInd-1; i>=startInd; i--) {
        output = (output % 140009)*x + int(id[i]); //note: casting a numerical char to int gives a value shifted by 48
        //did 140009 to prevent overflow
    } //polynomial this way so that later digits become more impactful
    return output;
}

int hashF(std::string id) {
    int a = horner (id, 3, 0, 3);
    int b = horner (id, 7, 4, 10); 
    int c = horner (id, 11, 12, 21);
    return (a+b+c) % 140009;
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
    

    vector<bool> occurred(140009, 0);
    srand(time(0));
    cout << occurred.size();
    int collisions = 0;

    for (int i = 0; i < 100000; i++) {
        string* myId = new string;
        *myId = randomId();
        cout << *myId << "\n";
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

//current: about 29% for 1L inputs