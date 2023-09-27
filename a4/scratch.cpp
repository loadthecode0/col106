#include <string>
#include<iostream>
using namespace std;

void f1() {
    string i1 = "12345678";
    string i2 = "-210326238453274534";

    int n1 = i1.size(); int offset1 = 0;
    int n2 = i2.size(); int offset2 = 0;
    
    if (i1[0] == '-') {n1 -= 1; offset1 = 1;}
    if (i2[0] == '-') {n2 -= 1; offset2 = 1;}

    int cap1 = 0; int cap2 = 0;

    if (n1%4 == 0) {cap1 = n1/4;} else {cap1 = 1 + (n1/4);}
    if (n2%4 == 0) {cap2 = n2/4;} else {cap2 = 1 + (n2/4);}

    int arr1[cap1] = {0}; int arr2[cap2] = {0};
    cout << n1 << " " << n2 << " " << cap1 << " " << cap2 << " \n";

    int j = offset1;
    while (j<= n1 - 1 + offset1) {
        int slot = cap1 - 1 - ((j-offset1)/4);
        arr1[slot] = 10*(arr1[slot]) + int(i1[j]) - 48;
        j++;
    }

    for (int i = 0; i<cap1; i++) {
        cout << arr1[i] << " ";
    }
    cout << "\n";

    int offset22 = 4 - (n2%4);
    int k =  offset2;
    while (k<= n2 - 1 + offset2) {
        int slot = cap2 - 1 - ((k-offset2 + offset22)/4);
        arr2[slot] = 10*(arr2[slot]) + int(i2[k]) - 48;
        k++;
    }

    for (int i = 0; i<cap2; i++) {
        cout << arr2[i] << " ";
    }
    cout << "\n";
}

string addMagnitude (int* arr1, int* arr2, int sz1, int sz2) {
    string s = "";
    int t = 0;
    int carry = 0;
    int index = 0; 
    int bigSize; int smallSize;

    if (sz1 > sz2) {
        bigSize = sz1; smallSize = sz2;
        while (index < smallSize) {
            int t = arr1[index] + arr2[index] + carry;
            if (t > 9) {carry = 1; t -= 10;} 
            else {carry = 0;}
            s = char(t+48) + s;
            index++; 
        } //now index = smallSize; carry may be 0 or 1
        while (index < bigSize) {
            int t = arr1[index] + carry;
            if (t > 9) {carry = 1; t -= 10;} 
            else {carry = 0;}
            s = char(t+48) + s;
            index++;
        } ;//now index = sz1
        if (carry == 1) {;
            s = '1' + s;
        }
    } else {
        bigSize = sz2; smallSize = sz1;
        while (index < smallSize) {
            int t = arr1[index] + arr2[index] + carry;
            if (t > 9) {carry = 1; t -= 10;} 
            else {carry = 0;}
            s = char(t+48) + s;
            index++; 
        } //now index = smallSize; carry may be 0 or 1
        while (index < bigSize) {
            int t = arr2[index] + carry;
            if (t > 9) {carry = 1; t -= 10;} 
            else {carry = 0;}
            s = char(t+48) + s;
            index++;
        } //now index = sz1
        if (carry == 1) {
            s = '1' + s;
        }
    }
    
    
    // cout << s << "  hello5\n";
    // cout << big << " " << small << " \n";
    // for (int i = 0; i<bigSize; i++) {
    //     cout << big[i] << " ";
    // } cout << "\n";
    // for (int i = 0; i<smallSize; i++) {
    //     cout << small[i] << " ";
    // } cout << "\n";
    // delete [] small; cout << s << "  hello6\n";
    // cout << big << " " << small << " \n";
    
    // for (int i = 0; i<smallSize; i++) {
    //     cout << small[i] << " ";
    // } cout << "\n";
    // for (int i = 0; i<bigSize; i++) {
    //     cout << big[i] << " ";
    // } cout << "\n";
    // delete [] big; 
    // cout << s << "  hello7\n";

    
    return s;
}

int compare (int* arr1, int* arr2, int sz1, int sz2) {
    if (sz1 > sz2) {
        return 1;
    } else if (sz2 > sz1) {
        return -1;
    } else { //sz1 = sz2 now
        int index = sz1 - 1;
        while (index >= 0) {
            if (arr1[index] > arr2[index]) {
                return 1;
            } else if (arr1[index] < arr2[index]) {
                return -1;
            } else {  //here arr1[index] == arr2[index]
                index--;
            }
        }
        return 0;
    }
}

string subSmallFromBig (int* arr1, int* arr2, int bigSize, int smallSize) { //tested, works
    //condition: num(arr1) > num(arr2), so sz1 >=  sz2
    string s = "";
    int t = 0;
    int carry = 0; //will be -1 in case of borrow
    int index = 0; 

    while (index < smallSize) {
        int t = arr1[index] - arr2[index] + carry;
        if (t < 0) {carry = -1; t += 10;} 
        else {carry = 0;}
        s = char(t+48) + s;
        index++; 
    } //now index = smallSize; carry may be 0 or -1
    while (index < bigSize) {
        int t = arr1[index] + carry;
        if (t < 0) {carry = -1; t += 10;} 
        else {carry = 0;}
        s = char(t+48) + s;
        index++;
    } //now index = bigSize, by hypothesis carry has to be 0. There will be exactly bigSize chars in s now.

    cout << s << "\n";

    //now delete leading zeroes
    int j = 0; string out = "";

    while (j < bigSize-1 && s[j] == '0') {
        j++;
    } //landed on MSD OR unit place if all zeroes.

    while (j < bigSize) {
        out = out + s[j];
        j++;
    }

    return out;
}

void f2 () {

    int arr1[5] = {9, 9, 9, 9, 4};
    int arr2[5] = {9, 9, 9, 0, 4};

    cout << subSmallFromBig (arr1, arr2, 5, 5) << "\n";

}

void f3 () {

    cout << (-57834582)%22;
}

int main()
{
    f3();
    

    return 0;
}