// Do NOT add any other includes
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

// from 35th element of the ascii table
class para_nodes
{
public:
    int book_code;
    int page;
    int paragraph;
    long long int para_frequency;

    para_nodes();
    para_nodes(int b_code, int pg, int para);
    ~para_nodes();
};

class TrieNode
{
public:
    TrieNode **children;

    bool EndOfWord;
    long long int frequency;
    vector<para_nodes *> existence = {};

    TrieNode();
    TrieNode(long long int freq, bool eow);

    ~TrieNode();
};

class Trie
{
    TrieNode *root;

public:
    Trie();
    ~Trie();
    void insert(string word, long long int count, int b_code, int pg, int para);
    TrieNode *search(string word);
};

class Dict
{
public:
    Trie *dictionary;

    // You can add attributes/helper functions here
    // hashtable* dictionary3;
    // RBTree* dictionary;
    // AvlTree* dictionary2;
    // int size= 10000;
    /* Please do not touch the attributes and
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    Dict();

    ~Dict();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence, long long int count, int &num_word);

    TrieNode *get_word_info(string word);

    long long int get_word_count(string word);
    /* -----------------------------------------*/
};
