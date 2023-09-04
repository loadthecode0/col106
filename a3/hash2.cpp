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


int binary(int num) {
    int bin = 0;
    int rem = 0;
    int q = 0;
    int iterations = 0;
    int multiplier = 1;
    while (num > 0) {
        rem = num % 2;
        bin = bin + rem * (multiplier);
        multiplier*= 10;
        num = num/2;
    }
    return bin;
}

int main()
{
    // int y = 109;
    // cout << binary(y) << "\n";

    int8_t x = 0x6d;
    printf("%x %d\n", x, x);
    cout << binary(109) << "\n";
    // int l = ((x>>2) | (x<<6));
    // printf("%x\n" , l );

    // int z = 0x1b5b;
    // printf("%x %d\n", z, z);

    // int w = 7003;
    // cout << binary(w) << "\n";

    int8_t y = x >> 2;
    printf("%x %d\n", y, y);

    int8_t z = x << 6;
    printf("%x %d\n", z, z);




    return 0;
}