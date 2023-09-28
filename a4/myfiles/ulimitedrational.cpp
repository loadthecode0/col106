/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "ulimitedrational.h"

//DELETE THESE!
#include <iostream>
using namespace std;
//DELETE THESE!

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

string gcdMagnitude(const string &s1, const string &s2) {
    
}

UnlimitedRational::UnlimitedRational(UnlimitedInt* num, UnlimitedInt* den) {
    int s1 = num->get_sign(); int s2 = den->get_sign();
    if (s2 != 0) {
        string mag1 = magnitude2(num->to_string());
        string mag2 = magnitude2(den->to_string());
        if (s1*s2 >= 0) {
            p = new UnlimitedInt(mag1);
            q = new UnlimitedInt(mag2);
            //reduce to gcd now
        } else {
            p = new UnlimitedInt("-" + mag1);
            q = new UnlimitedInt(mag2);
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
static UnlimitedRational* add(UnlimitedRational* i1, UnlimitedRational* i2);
static UnlimitedRational* sub(UnlimitedRational* i1, UnlimitedRational* i2);
static UnlimitedRational* mul(UnlimitedRational* i1, UnlimitedRational* i2);
static UnlimitedRational* div(UnlimitedRational* i1, UnlimitedRational* i2);

int main () {
    
    string x = "10";
    string y = "-5";

    UnlimitedInt *m = new UnlimitedInt(x);
    UnlimitedInt *n = new UnlimitedInt(y);

    UnlimitedRational *r = new UnlimitedRational(m, n);
    UnlimitedInt *s = UnlimitedInt::mod(m, n);

    std::cout <<r->get_p_str() << " " << r->get_q_str() << " " << r -> get_frac_str() << "\n";
    std:: cout << s->to_string() << "\n";

    return 0;
}