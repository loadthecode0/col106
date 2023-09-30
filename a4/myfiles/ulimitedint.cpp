/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "ulimitedint.h"

//DELETE THESE!
#include <iostream>
using namespace std;
//DELETE THESE!


//maintain that size always gives exact num of digits (no leading zeros)

UnlimitedInt::UnlimitedInt() { //default constructor
    unlimited_int = new int[1];
    unlimited_int[0] = 0;
    size = 1;
    capacity = 1;
    sign = 0;
}

UnlimitedInt::UnlimitedInt(string s) { //string constructor
    size = s.length(); int offset = 0; sign = 1; 
    if (s == "0") {sign = 0;}
    if (s[0] == '-') {size -= 1; offset = 1; sign = -1;}
    capacity = size;

    unlimited_int = new int[size];
    fill(unlimited_int, unlimited_int + size, 0);
    
    int j = offset;
    while (j<= size - 1 + offset) {
        unlimited_int[size - 1 + offset - j] = int(s[j]) - 48;
        j++;
    }
}

UnlimitedInt::UnlimitedInt(int i) { //integer constructor
    unlimited_int = new int[1];
    unlimited_int[0] = 0;
    size = 1;
    capacity = 1;
    sign = 0;
}

UnlimitedInt::UnlimitedInt(int* ulimited_int, int cap, int sgn, int sz) { // Complete constructor
    unlimited_int = ulimited_int;
    size = sz;
    capacity = cap;
    sign = sgn;
} 

int UnlimitedInt::get_size() { 
    return size;
}

int* UnlimitedInt::get_array() { 
    return unlimited_int;
}

int UnlimitedInt::get_sign() { 
    return sign;
}

int UnlimitedInt::get_capacity() { 
    return capacity;
}

UnlimitedInt::~UnlimitedInt() { //destructor
    size = 0;
    capacity = 0;
    delete [] unlimited_int;
}

string magnitude(string k) {
    if (k[0] == '-') {        
        string out = "";
        for (int i = 1; i<k.length(); i++) {
            out = out + k[i];
        }
        return out;
    } else {
        return k;
    }    
}

int compareMagnitude (int* arr1, int* arr2, int sz1, int sz2) { //tested, works; gives 1 if arr1 is bigger, -1 if arr2 is bigger, 0 if equal
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

int compareMagnitudeString (const string &s1, const string &s2) { //tested, works; gives 1 if arr1 is bigger, -1 if arr2 is bigger, 0 if equal
    //pre cond: s1, s2 are positive numbers, ie magnitudes already
    int sz1 = s1.length(); int sz2 = s2.length();
    if (sz1 > sz2) {
        return 1;
    } else if (sz2 > sz1) {
        return -1;
    } else { //sz1 = sz2 now
        int index = sz1 - 1;
        while (index >= 0) {
            if (s1[sz1 - 1 - index] > s2[sz2 - 1 - index]) {
                return 1;
            } else if (s1[sz1 - 1 - index] < s2[sz2 - 1 - index]) {
                return -1;
            } else {  //here both chars are equal
                index--;
            }
        }
        return 0;
    }
}

int compareMagnitudeUnInt (UnlimitedInt* i1, UnlimitedInt* i2) {

    int* arr1 = i1 -> get_array(); int* arr2 = i2 -> get_array();
    int sz1 = i1 -> get_size(); int sz2 = i2->get_size();

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

string addMagnitude (int* arr1, int* arr2, int sz1, int sz2) { //tested, works
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
        } //now index = bigSize, add carry if left.
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
    return s;
}

string addMagnitudeString (const string &s1, const string &s2) { //tested, works
    int sz1 = s1.length(); int sz2 = s2.length();
    string s = "";
    int t = 0;
    int carry = 0;
    int index = 0; 
    int bigSize; int smallSize;
    string bigString; string smallString;

    if (sz1 > sz2) {
        bigSize = sz1; smallSize = sz2;
        bigString = s1; smallString = s2;
    } else {
        bigSize = sz2; smallSize = sz1;
        bigString = s2; smallString = s1;
    }

    while (index < smallSize) {
            int t = int (bigString[bigSize - 1 - index]) + int(smallString[smallSize - 1 - index]) + carry - 96;
            if (t > 9) {carry = 1; t -= 10;} 
            else {carry = 0;}
            s = char(t+48) + s;
            index++; 
        } //now index = smallSize; carry may be 0 or 1
        while (index < bigSize) {
            int t = int (bigString[bigSize - 1 - index]) + carry - 48;
            if (t > 9) {carry = 1; t -= 10;} 
            else {carry = 0;}
            s = char(t+48) + s;
            index++;
        } //now index = bigSize, add carry if left.
        if (carry == 1) {;
            s = '1' + s;
        }  
    return s;
}

string subSmallFromBig (int* arr1, int* arr2, int bigSize, int smallSize) { //tested, works
    //pre-condition: num(arr1) > num(arr2), so sz1 >=  sz2
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

string subStringSmallFromBig (const string &s1, const string &s2) { //tested, works
    int bigSize = s1.length(); int smallSize = s2.length();
    //pre-condition: num(s1) > num(s2), so sz1 >=  sz2
    string s = "";
    int t = 0;
    int carry = 0;
    int index = 0; 

    while (index < smallSize) {
        int t = int(s1[bigSize - 1 - index]) - int (s2[smallSize - 1 - index]) + carry;
        if (t < 0) {carry = -1; t += 10;} 
        else {carry = 0;}
        s = char(t+48) + s;
        index++; 
    } //now index = smallSize; carry may be 0 or -1
    while (index < bigSize) {
        int t = int(s1[bigSize - 1 - index]) + carry -48;
        if (t < 0) {carry = -1; t += 10;} 
        else {carry = 0;}
        s = char(t+48) + s;
        index++;
    } //now index = bigSize, by hypothesis carry has to be 0. There will be exactly bigSize chars in s now.
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

string mulMagnitude (int* arr1, int* arr2, int sz1, int sz2) { //
    //called only if arr1 >= arr2 ie sz1>=sz2
    string s = "";
    int t = 0; //current sum
    int carry = 0;
    
    for (int i = 0; i < (sz1 + sz2 - 1); i++){
        if (i < sz2) {
            t = carry;
            for (int j = 0; j <= i; j++){
                t += (arr2[j]) * (arr1[i-j]);
            }
            if (t > 9) {carry = (t/10); t %= 10;} 
            else {carry = 0;}
            s = char(t+48) + s;
        } 
        else if (sz2 <= i && i < sz1) { //i >= sz1
            t = carry;
            for (int j = 0; j <= (sz2 - 1); j++){
                t += (arr2[j]) * (arr1[i-j]);
            }
            if (t > 9) {carry = (t/10); t %= 10;} 
            else {carry = 0;}
            s = char(t+48) + s;
        } else { //sz1 <= i
            t = carry; 
            for (int j = (i - sz1 + 1); j <= (sz2 - 1); j++){
                t += (arr2[j]) * (arr1[i-j]);
            }
            if (t > 9) {carry = (t/10); t %= 10;} 
            else {carry = 0;}
            s = char(t+48) + s;
        }  
        // std :: cout << s << "\n";
    }

    if (carry > 0) {
        s = char(carry+48) + s;
    }

    return s;
}
struct divResult
{
    string q;
    string r;
    divResult (string a, string b) { //constructor
        q = a; r = b;
    }
};

divResult divString(const string &s1, const string &s2) {
    string quotient = "0";
    string remainder = s1;
    int n = (s1.length() - s2.length());

    string divisor = s2; string quotientAdder = "1";
    while (n>0) {
        divisor += "0"; quotientAdder += "0"; n--;
    } //we will use these strings for generating divisors and quotient counter

    while (compareMagnitudeString(remainder, s2) != -1) { //at the end remainder has to strictly smaller than divisor
        // subtract repeatedly
        while (compareMagnitudeString(remainder, divisor) != -1) { //ie until remainder is smaller than divisor
            remainder = subStringSmallFromBig(remainder, divisor);
            quotient = addMagnitudeString(quotient, quotientAdder);
            
        } //now, remainder<=divisor
        //make prep for next iteration;
        divisor.pop_back(); quotientAdder.pop_back();
    }
    return divResult(quotient, remainder);
}

UnlimitedInt* UnlimitedInt::add(UnlimitedInt* i1, UnlimitedInt* i2) {
    int s1 = i1 -> get_sign(); int s2 = i2 -> get_sign();

    if (s1 == 0) {
        UnlimitedInt* x = new UnlimitedInt(i2 -> to_string());
        return x; //works if both s1 s2 are zero too
    } else if (s2 == 0) {
        UnlimitedInt* x = new UnlimitedInt(i1 -> to_string());
        return x;
    } else if (s1 * s2 > 0) { //nonzero, same sign

        int* arr1 = i1 -> get_array(); int* arr2 = i2 -> get_array();
        int sz1 = i1 -> get_size(); int sz2 = i2 -> get_size();

        string magnitude = addMagnitude(arr1, arr2, sz1, sz2);
        if (s1 < 0 ) { //both negative
            UnlimitedInt* x = new UnlimitedInt("-" + magnitude);
            return x;
        } else { // both positive
            UnlimitedInt* x = new UnlimitedInt(magnitude);
            return x;
        }
    } else { //nonzero, opposite signs
        
        int* arr1 = i1 -> get_array(); int* arr2 = i2 -> get_array();
        int sz1 = i1 -> get_size(); int sz2 = i2 -> get_size();
        int comp = compareMagnitude(arr1, arr2, sz1, sz2);

        if (comp == 1) {
            string magnitude = subSmallFromBig(arr1, arr2, sz1, sz2);
            if (s1 < 0) {
                UnlimitedInt* x = new UnlimitedInt("-" + magnitude); 
                return x; 
            } else {
                UnlimitedInt* x = new UnlimitedInt(magnitude); 
                return x; 
            }         
        } else if (comp == -1) {
            string magnitude = subSmallFromBig(arr2, arr1, sz2, sz1);
            if (s2 < 0) {
                UnlimitedInt* x = new UnlimitedInt("-" + magnitude); 
                return x; 
            } else {
                UnlimitedInt* x = new UnlimitedInt(magnitude); 
                return x; 
            } 
        } else { //comp == 0, equal.
            UnlimitedInt* x = new UnlimitedInt("0"); 
            return x;
        }
    }
    
}

UnlimitedInt* UnlimitedInt::sub(UnlimitedInt* i1, UnlimitedInt* i2) {
    int s1 = i1 -> get_sign(); int s2 = i2 -> get_sign();
    string mag1 = magnitude(i1->to_string());
    string mag2 = magnitude(i2->to_string());

    if (s2 == 0) {
        return new UnlimitedInt(i1 -> to_string());; //works if both s1 s2 are zero too
    } else if (s1 == 0) { //s1 == 0 and s2 != 0
        if (s2 > 0) {
            return new UnlimitedInt("-" + mag2);;
        } else {
            return new UnlimitedInt(mag2);
        }
        //now, none of s1, s2 can be "0"

    } else if (s1 * s2 < 0) { //nonzero, opposite signs

        string magnitude = addMagnitudeString(mag1, mag2);

        if (s1 < 0) { //and s2>0
            return new UnlimitedInt("-" + magnitude);
        } else {
            return new UnlimitedInt(magnitude);
        }            
        
    } else { //nonzero, same sign
        
        int comp = compareMagnitudeString(mag1, mag2);

        if (comp == 1) { //|i1| > |i2|
            string magnitude = subStringSmallFromBig(mag1, mag2);
            if (s1 < 0) { //and s2<0
                return new UnlimitedInt("-" + magnitude); 
            } else { //s1, s2 > 0
                return new UnlimitedInt(magnitude); 
            }         
        } else if (comp == -1) { //|i1| < |i2|
            string magnitude = subStringSmallFromBig(mag2, mag1);
            if (s2 < 0) {
                return new UnlimitedInt(magnitude); 
            } else {
                return new UnlimitedInt("-" + magnitude); 
            } 
        } else { //comp == 0, equal.
            return new UnlimitedInt("0");
        }
    } 
}

UnlimitedInt* UnlimitedInt::mul(UnlimitedInt* i1, UnlimitedInt* i2) {
    int s1 = i1 -> get_sign(); int s2 = i2 -> get_sign();
    int* arr1 = i1 -> get_array(); int* arr2 = i2 -> get_array();
    int sz1 = i1 -> get_size(); int sz2 = i2 -> get_size();
    int comp = compareMagnitude(arr1, arr2, sz1, sz2);
    string magnitude = "";

    if (s1 * s2 == 0) {
        UnlimitedInt* x = new UnlimitedInt("0");
        return x;
    } else {
        if (comp == -1) {  //arr2 < arr1 strictly
            magnitude = mulMagnitude(arr2, arr1, sz2, sz1);
        } else { //arr1 >= arr2
            magnitude = mulMagnitude(arr1, arr2, sz1, sz2);
        }

        if (s1*s2 > 0) {
            UnlimitedInt* x = new UnlimitedInt(magnitude);
            return x;
        } else { //s1*s2 < 0
            UnlimitedInt* x = new UnlimitedInt("-" + magnitude);
            return x;
        } 
    }
}

UnlimitedInt* UnlimitedInt::div(UnlimitedInt* i1, UnlimitedInt* i2) { //tested, works
    
    string mag1 = magnitude(i1->to_string());
    string mag2 = magnitude(i2->to_string());

    int s1 = i1 -> get_sign(); int s2 = i2 -> get_sign();

    if (s2 != 0) {
        if (s1 == 0) {
            return new UnlimitedInt("0");
        }
        else  {
            divResult x = divString(mag1, mag2);
            string divMag = x.q;
            if (s1*s2 > 0) {
                return new UnlimitedInt(divMag);
            } else {
                return new UnlimitedInt("-" + divMag);
            }
        } 
    }
}

UnlimitedInt* UnlimitedInt::mod(UnlimitedInt* i1, UnlimitedInt* i2) { //tested, works
    
    string mag1 = magnitude(i1->to_string());
    string mag2 = magnitude(i2->to_string());

    int s1 = i1 -> get_sign(); int s2 = i2 -> get_sign();

    if (s2 != 0) {
        if (s1 == 0) {
            return new UnlimitedInt("0");
        }
        else  {
            divResult x = divString(mag1, mag2);
            string rem = x.r;
            string rem2 = subStringSmallFromBig(mag2, rem);
            if (rem == "0") {
                return new UnlimitedInt("0");
            } else if (s1 > 0 && s2 > 0) { //nonzero rem
                return new UnlimitedInt(rem);
            } else if (s1 < 0 && s2 > 0) { //nonzero rem
                return new UnlimitedInt(rem2);
            } else if (s1 > 0 && s2 < 0) { //nonzero rem
                return new UnlimitedInt("-" + rem2);
            } else { //nonzero rem, both signs negative
                return new UnlimitedInt("-" + rem);
            }
        } 
    }
}

string UnlimitedInt::to_string() {
    string s = "";
    if (sign == -1) {s += "-";}
    for (int i = size-1; i>=0; i--) {
        s += char(unlimited_int[i] + 48); 
    }
    return s;
}





// int main() {
//     // string s = "12402";
//     // std::cout << "hello1\n";
//     // UnlimitedInt* x = new UnlimitedInt(s);
//     // std::cout << "hello2\n";
//     // int n = x->get_size();
//     // int* arr = x->get_array();
//     // int cap = x->get_capacity();
//     // std::cout << x -> get_size() << " " << x-> get_array() << " " << x->get_sign() <<" " << x->get_capacity() << "\n";

//     // for (int i = 0; i<cap; i++) {
//     //     std::cout << arr[i] << " ";
//     // }
//     // std::cout << "\n";

//     // std::cout << x->to_string();

//     string y = "72";
//     string z = "0";

//     UnlimitedInt* a = new UnlimitedInt(y);
//     UnlimitedInt* b = new UnlimitedInt(z);

//     std::cout << a->to_string() << "\n";
//     std::cout << b->to_string() << " " << b->get_sign() << "\n";


//     UnlimitedInt* c = new UnlimitedInt();

//     c = UnlimitedInt::mul(a, b);

//     std::cout << c->to_string() << "\n";
//     // std::cout << magnitude(z);

//     // divResult trial = divString("9354739941", "29939");

//     // cout << trial.r << "\n";

//     // string w = y;

//     // w = subStringSmallFromBig(w, z);

//     // std::cout << y << "\n";

//     // return 0;
// }