/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "entry.h"

SymEntry::SymEntry() {
    key = "newEntry"; //new root key approx in the middle of alphabet
    val = new UnlimitedRational();
    left = nullptr;
    right = nullptr;
}

SymEntry::SymEntry(string k, UnlimitedRational* v) {
    key = k;
    val = v;
    left = nullptr;
    right = nullptr;
}

SymEntry::~SymEntry() {
    delete left; left = nullptr;
    delete right; right = nullptr;
}