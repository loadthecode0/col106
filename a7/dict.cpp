#include "dict.h"
string convertToLowerCase2(string word)
{
    string newWord = "";
    for (int i = 0; i < word.length(); i++)
    {
        char x = word[i];
        newWord.push_back((char)tolower(x));
    }
    return newWord;
}
para_nodes::para_nodes()
{
}

para_nodes::para_nodes(int b_code, int pg, int para)
{
    book_code = b_code;
    page = pg;
    paragraph = para;
    para_frequency = 1;
}
para_nodes::~para_nodes()
{
}

TrieNode::TrieNode()
{
    children = new TrieNode *[92];
    for (int i = 0; i < 92; i++)
    {
        children[i] = nullptr;
    }
}

TrieNode::TrieNode(long long int freq, bool eow)
{
    children = new TrieNode *[92];
    for (int i = 0; i < 92; i++)
    {
        children[i] = nullptr;
    }
    frequency = freq;
    EndOfWord = eow;
}

TrieNode::~TrieNode()
{
    for (int i = 0; i < 92;i++) {
        if (children[i] != nullptr) {
            delete children[i];
            children[i] = nullptr;
        }
    }

    delete[] children;
    children = nullptr;

    for (int i = 0; i < existence.size();i++) {
        if (existence[i] != nullptr) {
            delete existence[i];
            existence[i] = nullptr;
        }
    }
}
Trie::Trie()
{
    root = new TrieNode();
}

Trie::~Trie()
{
    if (root != nullptr)
    {
        delete root;
        root = nullptr;
    }
}

void Trie::insert(string word, long long count, int b_code, int pg, int para)
{
    TrieNode *temp = root;
    for (int i = 0; i < word.size() - 1; i++)
    {
        int tar = word[i] - 35;
        if (temp->children[tar] == nullptr)
        {
            temp->children[tar] = new TrieNode(0, false);
        }
        temp = temp->children[tar];
    }
    int tar = word[word.size() - 1] - 35;
    if (temp->children[tar] == nullptr)
    {
        temp->children[tar] = new TrieNode(count, true);
        para_nodes *curr = new para_nodes(b_code, pg, para);
        temp->children[tar]->existence.push_back(curr);
    }
    else
    {
        temp->children[tar]->EndOfWord = true;

        if (count == 1)
            temp->children[tar]->frequency++;
        else
            temp->children[tar]->frequency = count;
        if (temp->children[tar]->existence.size() != 0)
        {
            if (temp->children[tar]->existence.back()->book_code == b_code && temp->children[tar]->existence.back()->page == pg && temp->children[tar]->existence.back()->paragraph == para)
            {
                temp->children[tar]->existence.back()->para_frequency++;
            }
            else
            {
                para_nodes *curr = new para_nodes(b_code, pg, para);
                temp->children[tar]->existence.push_back(curr);
            }
        }
        else
        {
            para_nodes *curr = new para_nodes(b_code, pg, para);
            temp->children[tar]->existence.push_back(curr);
        }
    }
}

TrieNode *Trie::search(string ori_word)
{
    string word=convertToLowerCase2(ori_word);
    TrieNode *temp = root;

    for (int i = 0; i < word.size() - 1; i++)
    {
        if (word[i] == ' ')
        {
            return NULL;
        }
        int tar = word[i] - 35;
        temp = temp->children[tar];
        if (temp == nullptr)
        {

            return nullptr;
        }
    }
    int tar = word[word.size() - 1] - 35;
    temp = temp->children[tar];
    if (temp != nullptr && temp->EndOfWord == true)
        return temp;

    return nullptr;
}
// returning nullptr in case word not found in the dictionary

Dict::Dict()
{
    dictionary = new Trie();
}

Dict::~Dict()
{
    if (dictionary != nullptr)
    {
        delete dictionary;
        dictionary = nullptr;
    }
}

bool is_separator(string separator, char check)
{
    for (int i = 0; i < separator.size(); i++)
    {
        if (check == separator[i])
            return true;
    }
    return false;
}

void Dict::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence, long long int count, int &num_word)
{
    string separators = " .,-:!\"\'()?[];@";
    string word = "";
    for (int i = 0; i < sentence.size(); i++)
    {
        if (is_separator(separators, sentence[i]))
        {
            if (word != "")
            {
                dictionary->insert(word, count, book_code, page, paragraph);
                num_word++;
            }
            word = "";
        }
        else
        {
            if (sentence[i] >= 'A' && sentence[i] <= 'Z')
            {
                sentence[i] += 32;
            }
            word += sentence[i];
        }
    }
    if (word != "")
    {
        dictionary->insert(word, count, book_code, page, paragraph);
        num_word++;
    }
    return;
}

TrieNode *Dict::get_word_info(string word)
{
    return dictionary->search(word);
}

long long int Dict::get_word_count(string word)
{
    TrieNode *temp = dictionary->search(word);
    if (temp == nullptr)
        return 0;
    return temp->frequency;
}