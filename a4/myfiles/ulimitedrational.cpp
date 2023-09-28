/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "ulimitedrational.h"

//DELETE THESE!
#include <iostream>
using namespace std;
//DELETE THESE!

//utility functions with strings

string magnitude2(string k) {
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

int compareMagnitudeString2 (const string &s1, const string &s2) { //tested, works; gives 1 if arr1 is bigger, -1 if arr2 is bigger, 0 if equal
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

string addMagnitudeString2 (const string &s1, const string &s2) { //tested, works
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

string subStringSmallFromBig2 (const string &s1, const string &s2) { //tested, works
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

string mulMagnitude2 (int* arr1, int* arr2, int sz1, int sz2) { //
    //called only if s1 >= s2 ie sz1>=sz2
    
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
    }

    if (carry > 0) {
        s = char(t+48) + s;
    }

    return s;
}
struct divResult2
{
    string q;
    string r;
    divResult2 (string a, string b) { //constructor
        q = a; r = b;
    }
};

divResult2 divString2(const string &s1, const string &s2) {
    string quotient = "0";
    string remainder = s1;
    int n = (s1.length() - s2.length());

    string divisor = s2; string quotientAdder = "1";
    while (n>0) {
        divisor += "0"; quotientAdder += "0"; n--;
    } //we will use these strings for generating divisors and quotient counter

    while (compareMagnitudeString2(remainder, s2) != -1) { //at the end remainder has to strictly smaller than divisor
        // subtract repeatedly
        while (compareMagnitudeString2(remainder, divisor) != -1) { //ie until remainder is smaller than divisor
            remainder = subStringSmallFromBig2(remainder, divisor);
            quotient = addMagnitudeString2(quotient, quotientAdder);
            
        } //now, remainder<=divisor
        //make prep for next iteration;
        divisor.pop_back(); quotientAdder.pop_back();
    }
    return divResult2(quotient, remainder);
}

string gcdMagnitude(const string &s1, const string &s2) {//tested, works
    string x=s1; string y=s2;
    if (x == "0") {
        return y;
    }   
    divResult2 m = divString2(y,x);
    return gcdMagnitude(m.r, x);
}

//class member functions

UnlimitedRational::UnlimitedRational() {
    p = new UnlimitedInt("0");
    q = new UnlimitedInt("1");
}

UnlimitedRational::UnlimitedRational(UnlimitedInt* num, UnlimitedInt* den) { //tested, works
    int s1 = num->get_sign(); int s2 = den->get_sign();
    if (s2 != 0) {
        string mag1 = magnitude2(num->to_string());
        string mag2 = magnitude2(den->to_string());
        string g = gcdMagnitude(mag1, mag2);
        string n = divString2(mag1, g).q;
        string d = divString2(mag2, g).q;
        if (s1*s2 >= 0) {
            p = new UnlimitedInt(n);
            q = new UnlimitedInt(d);
        } else {
            p = new UnlimitedInt("-" + n);
            q = new UnlimitedInt(d);
        }
    }
}

UnlimitedRational::~UnlimitedRational() {
    delete p;
    delete q;
}

UnlimitedInt* UnlimitedRational::UnlimitedRational::get_p() {
    return p;
}

UnlimitedInt* UnlimitedRational::get_q() {
    return q;
}

string UnlimitedRational::get_p_str() {
    return p->to_string();
}

string UnlimitedRational::get_q_str() {
    return q->to_string();
}

string UnlimitedRational::get_frac_str() {
    return (p->to_string()+ "/" + q->to_string());
}

//tested till here, works

// Rational arithmetic operations
UnlimitedRational* UnlimitedRational::add(UnlimitedRational* i1, UnlimitedRational* i2) {
    //read input
    UnlimitedInt* p1 = i1->get_p(); UnlimitedInt* p2 = i2->get_p();
    string q1 = i1->get_q_str(); string q2 = i2->get_q_str();

    string x = gcdMagnitude(q1, q2); //we have ensured that denoms of rationals are always positive
    UnlimitedInt* a = new UnlimitedInt(divString2(q1, x).q); 
    UnlimitedInt* b = new UnlimitedInt(divString2(q2, x).q); 

    UnlimitedInt* num = UnlimitedInt::add(UnlimitedInt::mul(p1, b), UnlimitedInt::mul(p2, a) );
    UnlimitedInt* den = UnlimitedInt::mul(i1->get_q(), b);
    cout << i1->get_q()->to_string() << "\n";

    delete a; delete b;

    return new UnlimitedRational(num, den);
}

UnlimitedRational* UnlimitedRational::sub(UnlimitedRational* i1, UnlimitedRational* i2) {
    //read input
    UnlimitedInt* p1 = i1->get_p(); UnlimitedInt* p2 = i2->get_p();
    string q1 = i1->get_q_str(); string q2 = i2->get_q_str();

    string x = gcdMagnitude(q1, q2); //we have ensured that denoms of rationals are always positive
    UnlimitedInt* a = new UnlimitedInt(divString2(q1, x).q); 
    UnlimitedInt* b = new UnlimitedInt(divString2(q2, x).q); 

    UnlimitedInt* num = UnlimitedInt::sub(UnlimitedInt::mul(p1, b), UnlimitedInt::mul(p2, a) );
    UnlimitedInt* den = UnlimitedInt::mul(i1->get_q(), b);
    cout << i1->get_q()->to_string() << "\n";

    delete a; delete b;

    return new UnlimitedRational(num, den);
}

UnlimitedRational* UnlimitedRational::mul(UnlimitedRational* i1, UnlimitedRational* i2) {
    UnlimitedInt* p1 = i1->get_p(); UnlimitedInt* p2 = i2->get_p();
    UnlimitedInt* q1 = i1->get_q(); UnlimitedInt* q2 = i2->get_q();
    return new UnlimitedRational (UnlimitedInt::mul(p1, p2), UnlimitedInt::mul(q1, q2));
}

UnlimitedRational* UnlimitedRational::div(UnlimitedRational* i1, UnlimitedRational* i2) {
    UnlimitedInt* p1 = i1->get_p(); UnlimitedInt* p2 = i2->get_p();
    UnlimitedInt* q1 = i1->get_q(); UnlimitedInt* q2 = i2->get_q();
    return new UnlimitedRational (UnlimitedInt::mul(p1, q2), UnlimitedInt::mul(q1, p2));
}

int main () {
    
    string x = "216";
    string y = "7";

    string z = "48";
    string w = "-8";

    UnlimitedInt *m = new UnlimitedInt(x);
    UnlimitedInt *n = new UnlimitedInt(y);
    UnlimitedInt *p = new UnlimitedInt(z);
    UnlimitedInt *q = new UnlimitedInt(w);

    UnlimitedRational *r = new UnlimitedRational(m, n);
    UnlimitedRational *s = new UnlimitedRational(p, q);

    std::cout <<r->get_p_str() << " " << r->get_q_str() << " " << r -> get_frac_str() << "\n";
    std::cout <<s->get_p_str() << " " << s->get_q_str() << " " << s -> get_frac_str() << "\n";
    // std:: cout << s->to_string() << "\n";
    UnlimitedRational *t = UnlimitedRational::div(r, s);
    cout << t->get_frac_str() << "\n";
    // std::cout << gcdMagnitude("0", "2325234") << "\n";


    return 0;
}