#include <bits/stdc++.h>
using namespace std;

bool t1(int x) {
    return x;
}

int main()
{



    vector<vector<int>> vec1 ;
    vec1.resize(5, {});
    cout << 1 << " " << vec1[2].size() << " " << typeid(vec1[2]).name() << "\n";
    vec1[2].push_back(2);
    cout << 1.1 << " " << vec1[2].size() << " " << typeid(vec1[2]).name() << "\n";

    vector<int> vec2;
    cout << 2 << " " << vec2.size()<< "\n";

    return 0;
}