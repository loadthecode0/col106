#include <bits/stdc++.h>
using namespace std;
#include "BaseClass.h"

int horner(const std::string& id, int x, int startInd, int endInd) {
    int output = int(id[endInd]); // Initialize output
    // Evaluate value of polynomial using Horner's method
    for (int i=endInd-1; i>=startInd; i--) {
        output = output*x + int(id[i]); //note: casting a numerical char to int gives a value shifted by 48
    } //polynomial this way so that later digits become more impactful
    return output;
}

int hashF(std::string id) {
    int a = horner (id, 3, 0, 3);
    int b = horner (id, 3, 4, 10); 
    int c = horner (id, 5, 12, 21);
    return (a+b+c) % 196613;
}

int main()
{



    string s = "BCJE2649204_1637294627"




    return 0;
}