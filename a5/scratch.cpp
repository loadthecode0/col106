#include <bits/stdc++.h>
using namespace std;

string myBin(int n) {
    int t = n;
    string s = "";
    while (t > 0) {
        s = to_string(t%2) + s;
        t /= 2;
    }
    return s;
}

int main()
{
    int y = 8;
    cout << myBin(y);
    
    return 0;
}