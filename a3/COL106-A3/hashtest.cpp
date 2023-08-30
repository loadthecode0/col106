#include <bits/stdc++.h>
#include <string>
using namespace std;

int horner(string id, int x, int startInd, int endInd) {
    int result = int(id[endInd]); // Initialize result
    // Evaluate value of polynomial using Horner's method
    for (int i=endInd-1; i>=startInd; i--) {
        result = result*x + int(id[i]); //note: casting a numerical char to int gives a value shifted by 48
    } //this way so that later digits become more impactful
    return result;
}



int main()
{

    string id = "SBIN0009835_9999999999";
    int a = horner (id, 3, 0, 3);
    int b = horner (id, 3, 4, 10); 
    int c = horner (id, 5, 12, 21);
    cout << a << " " << b << " " << c << " \n";
    cout << a+b+c << "\n";
    cout << (a+b+c) % 100000;

    return 0;
}