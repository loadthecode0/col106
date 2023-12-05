#include <assert.h>
#include <sstream>
#include "qna_tool.h"
#include <cmath> //included cmath header file
using namespace std;
#define c1 1.5
#define c2 0.5
using namespace std;

#define e 2.71828

long double AvgParalen = 0.0;
long long int no_of_words = 0;
long long int no_of_paras = 0;
int num_books{99};
int num_pages{537};
int num_para{79};

vector<vector<vector<long int>>> para_len(num_books, vector<vector<long int>>(num_pages, vector<long int>(num_para, 0)));
Node *BMScore(vector<vector<string>> Q, Trie *T, int k)
{
    AvgParalen = no_of_words / no_of_paras;
    vector<vector<vector<long double>>> table(num_books, vector<vector<long double>>(num_pages, vector<long double>(num_para, 0)));
    vector<vector<vector<bool>>> visited(num_books, vector<vector<bool>>(num_pages, vector<bool>(num_para, false)));
    vector<para_nodes *> to_be_sorted;
    for (auto v : Q)
    {   
        string q = v[0];
        double wt = stod(v[2]);
        TrieNode *bmNode = T->search(q);

        if (bmNode != NULL)
        {   
            for (auto p : bmNode->existence) // refers to each paragraph in which query q is present and update its score
            {
                int b_num = p->book_code;
                int pg_num = p->page;
                int p_num = p->paragraph;
                if (!visited[b_num][pg_num][p_num])
                {
                    visited[b_num][pg_num][p_num] = true;
                    para_nodes *pd=new para_nodes(b_num,pg_num,p_num);
                    to_be_sorted.push_back(pd);
                }

                

                long double score = 0.0;

                float n = bmNode->existence.size();
                float N = no_of_paras;
                float IDF = log((N - n + 0.5) / (n + 0.5));
                long int f = p->para_frequency;
                long double paraLen = para_len[b_num][pg_num][p_num];
                long double avgLen = AvgParalen;
                long double con = 1 - c2 + (c2 * (paraLen / AvgParalen));
                score = IDF * ((f * (c1 + 1)) / (f + (c1 * con))) * wt;

                table[b_num][pg_num][p_num] += score;
            }
        }
    }

    BinaryHeap H;
    vector<Master_para_node *> to_be_inserted;
    for (auto p_element : to_be_sorted)
    {
        int b_num = p_element->book_code;
        int pg_num = p_element->page;
        int p_num = p_element->paragraph;
        long double sc = table[b_num][pg_num][p_num];
        Master_para_node *m = new Master_para_node(b_num, pg_num, p_num, sc);
        to_be_inserted.push_back(m);
    }

    long long int sz = to_be_sorted.size();
    H.buildHeap(to_be_inserted, k);
    if (H.is_empty())
    {
        return NULL;
    }
    for (long long int j{k}; j < sz; j++)
    {
        if (H.heap[0]->para_score < to_be_inserted[j]->para_score)
        {
            H.heap[0] = to_be_inserted[j];
            H.heapifyDown(0);
        }
    }
    int i{0};
    Node *temp = NULL;
    while (i < k)
    {

        Master_para_node *min_element = H.minElem();

        H.deleteMin();

        Node *n1 = new Node(min_element->book_code, min_element->page, min_element->paragraph, 0, 0);
        if (temp == NULL && i == 0)
        {
            temp = n1;
            // head = n1;
        }
        else if (temp == NULL)
        {
            break;
        }
        else
        {
            temp->left = n1;
            n1->right = temp;
            temp = temp->left;
        }
        i++;
    }
    for (int i{0}; i < to_be_sorted.size(); i++)
    {
        delete to_be_sorted[i];
    }
    for (int i{0}; i < to_be_inserted.size(); i++)
    {
        delete to_be_inserted[i];
    }
    return temp;
}
Node *QNA_tool::get_top_k_para2(vector<vector<string>> question, int k)
{ 
    return BMScore(question, this->personal_dictionary->dictionary, k);
}
Master_para_node::Master_para_node()
{
}
Master_para_node::~Master_para_node()
{
    book_code = 0;
    page = 0;
    paragraph = 0;
    para_score = 0;
}

Master_para_node::Master_para_node(int b_code, int pg, int para, long double p_score)
{
    book_code = b_code;
    page = pg;
    paragraph = para;
    para_score = p_score;
}

bool Master_para_node::operator>(const Master_para_node &other) const
{
    return para_score > other.para_score;
}

QNA_tool::QNA_tool()
{
    personal_dictionary = new Dict();
    global_dictionary = new Dict();
    ifstream read_me("unigram_freq.csv");
    if (read_me.is_open())
    {
        string y;
        getline(read_me, y);
        string s;

        while (getline(read_me, s))
        {
            istringstream flow(s);
            string word;
            long long value;

            getline(flow, word, ',');
            flow >> value;
            int temp = 0;

            global_dictionary->insert_sentence(0, 0, 0, 0, word, value, temp);
        }
    }
    read_me.close();
}

QNA_tool::~QNA_tool()
{
    delete global_dictionary;
    delete personal_dictionary;
}

void QNA_tool::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence)
{
    int num_word = 0;
    personal_dictionary->insert_sentence(book_code, page, paragraph, sentence_no, sentence, 1, num_word);
    para_len[book_code][page][paragraph] += num_word;
    no_of_words += num_word;
    if (para_len[book_code][page][paragraph] == num_word)
    {
        no_of_paras += 1;
    }

    return;
}

bool is_separator_v2(string separator, char check)
{
    for (int i = 0; i < separator.size(); i++)
    {
        if (check == separator[i])
            return true;
    }
    return false;
}

Node *QNA_tool::get_top_k_para(string question, int k)
{
    vector<vector<vector<long double>>> ScoreStore(num_books, vector<vector<long double>>(num_pages, vector<long double>(num_para, 0)));
    vector<vector<vector<bool>>> visited_paras(num_books, vector<vector<bool>>(num_pages, vector<bool>(num_para, false)));
    vector<para_nodes *> to_be_sorted;
    vector<string> query_words;
    vector<long double> word_score;

    string separators = " .,-:!\"\'()?[];@";
    string word = "";

    for (int i = 0; i < question.size(); i++)
    {
        if (is_separator_v2(separators, question[i]))
        {
            if (word != "")
            {
                query_words.push_back(word);
                long double score = ((1 + static_cast<long double>(personal_dictionary->get_word_count(word))) * 100000) / (1 + static_cast<long double>(global_dictionary->get_word_count(word)));
                // cout << score << "score initial" << endl;
                word_score.push_back(score);
            }
            word = "";
        }
        else
        {
            if (question[i] >= 'A' && question[i] <= 'Z')
            {
                question[i] += 32;
            }
            word += question[i];
        }
    }
    if (word != "")
    {
        query_words.push_back(word);
        long double score = ((1 + static_cast<long double>(personal_dictionary->get_word_count(word))) * 100000) / (1 + static_cast<long double>(global_dictionary->get_word_count(word)));
        word_score.push_back(score);
    }

    // score paragraph and sort
    for (int i = 0; i < query_words.size(); i++)
    {
        TrieNode *refer = personal_dictionary->get_word_info(query_words[i]);
        if (refer != nullptr)
        {
            for (long long int j = 0; j < refer->existence.size(); j++)
            {
                ScoreStore[refer->existence[j]->book_code][refer->existence[j]->page][refer->existence[j]->paragraph] += word_score[i] * refer->existence[j]->para_frequency;
                
                if (visited_paras[refer->existence[j]->book_code][refer->existence[j]->page][refer->existence[j]->paragraph] == true)
                    continue;
                else
                {
                    visited_paras[refer->existence[j]->book_code][refer->existence[j]->page][refer->existence[j]->paragraph] = true;
                    para_nodes *pd=new para_nodes(refer->existence[j]->book_code,refer->existence[j]->page,refer->existence[j]->paragraph);
                    to_be_sorted.push_back(pd);
                }
            }
        }
    }

    BinaryHeap H;
    vector<Master_para_node *> to_be_inserted;
    for (auto p_element : to_be_sorted)
    {
        int b_num = p_element->book_code;
        int pg_num = p_element->page;
        int p_num = p_element->paragraph;
        long double sc = ScoreStore[b_num][pg_num][p_num];
        Master_para_node *m = new Master_para_node(b_num, pg_num, p_num, sc);
        to_be_inserted.push_back(m);
    }

    long long int sz = to_be_sorted.size();
    H.buildHeap(to_be_inserted, k);
    if (H.is_empty())
    {
        return NULL;
    }
    for (long long int j{k}; j < sz; j++)
    {
        if (H.heap[0]->para_score < to_be_inserted[j]->para_score)
        {
            H.heap[0] = to_be_inserted[j];
            H.heapifyDown(0);
        }
    }
    int i{0};
    Node *temp = NULL;
    while (i < k)
    {

        Master_para_node *min_element = H.minElem();
        H.deleteMin();

        Node *n1 = new Node(min_element->book_code, min_element->page, min_element->paragraph, 0, 0);
        if (temp == NULL && i == 0)
        {
            temp = n1;
        }
        else if (temp == NULL)
        {
            break;
        }
        else
        {
            temp->left = n1;
            n1->right = temp;
            temp = temp->left;
        }
        i++;
    }
    for (int i{0}; i < to_be_inserted.size(); i++)
    {
        delete to_be_inserted[i];
    }
    for (int i{0}; i < to_be_sorted.size(); i++)
    {
        delete to_be_sorted[i];
    }

    return temp;
}


long long int QNA_tool::freq(string &token)
{
    // return dict search value;

    return personal_dictionary->get_word_count(token);
}

string convertToLowerCase(string word)
{
    string newWord = "";
    for (int i = 0; i < word.length(); i++)
    {
        char x = word[i];
        newWord.push_back((char)tolower(x));
    }
    return newWord;
}

void QNA_tool::generateQueryString(vector<string> queryList, string &apiCall)
{

    vector<string> stopWords = {"what", "who", "where", "when", "how", "?", "!", ",", ";", ".", "0o", "0s", "3a", "3b", "3d", "6b", "6o", "a", "a1", "a2", "a3", "a4",
                                "ab", "able", "about", "above",
                                "abst", "ac", "accordance", "according", "accordingly", "across", "act", "actually", "ad", "added", "adj", "ae", "af", "affected",
                                "affecting", "affects", "after", "afterwards", "ag", "again", "against", "ah", "ain", "ain't", "aj", "al", "all", "allow", "allows",
                                "almost", "alone", "along", "already", "also", "although", "always", "am", "among", "amongst", "amoungst", "amount", "an", "and",
                                "announce", "another", "any", "anybody", "anyhow", "anymore", "anyone", "anything", "anyway", "anyways", "anywhere", "ao", "ap", "apart",
                                "apparently", "appear", "appreciate", "appropriate", "approximately", "ar", "are", "aren", "arent", "aren't", "arise", "around", "as", "a's",
                                "aside", "ask", "asking", "associated", "at", "au", "auth", "av", "available", "aw", "away", "awfully", "ax", "ay", "az", "b", "b1", "b2",
                                "b3", "ba", "back", "bc", "bd", "be", "became", "because", "become", "becomes", "becoming", "been", "before", "beforehand", "begin",
                                "beginning", "beginnings", "begins", "behind", "being", "believe", "below", "beside", "besides", "best", "better", "between", "beyond", "bi",
                                "bill", "biol", "bj", "bk", "bl", "bn", "both", "bottom", "bp", "br", "brief", "briefly", "bs", "bt", "bu", "but", "bx", "by", "c", "c1",
                                "c2", "c3", "ca", "call", "came", "can", "cannot", "cant", "can't", "cause", "causes", "cc", "cd", "ce", "certain", "certainly", "cf", "cg",
                                "ch", "changes", "ci", "cit", "cj", "cl", "clearly", "cm", "c'mon", "cn", "co", "com", "come", "comes", "con", "concerning", "consequently",
                                "consider", "considering", "contain", "containing", "contains", "corresponding", "could", "couldn", "couldnt", "couldn't", "course", "cp",
                                "cq", "cr", "cry", "cs", "c's", "ct", "cu", "currently", "cv", "cx", "cy", "cz", "d", "d2", "da", "date", "dc", "dd", "de", "definitely",
                                "describe", "described", "despite", "detail", "df", "di", "did", "didn", "didn't", "different", "dj", "dk", "dl", "do", "does", "doesn",
                                "doesn't", "doing", "don", "done", "don't", "down", "downwards", "dp", "dr", "ds", "dt", "du", "due", "during", "dx", "dy", "e", "e2", "e3",
                                "ea", "each", "ec", "ed", "edu", "ee", "ef", "effect", "eg", "ei", "eight", "eighty", "either", "ej", "el", "eleven", "else", "elsewhere",
                                "em", "empty", "en", "end", "ending", "enough", "entirely", "eo", "ep", "eq", "er", "es", "especially", "est", "et", "et-al", "etc", "eu",
                                "ev", "even", "ever", "every", "everybody", "everyone", "everything", "everywhere", "ex", "exactly", "example", "except", "ey", "f", "f2",
                                "fa", "far", "fc", "few", "ff", "fi", "fifteen", "fifth", "fify", "fill", "find", "fire", "first", "five", "fix", "fj", "fl", "fn", "fo",
                                "followed", "following", "follows", "for", "former", "formerly", "forth", "forty", "found", "four", "fr", "from", "front", "fs", "ft", "fu",
                                "full", "further", "furthermore", "fy", "g", "ga", "gave", "ge", "get", "gets", "getting", "gi", "give", "given", "gives", "giving", "gj",
                                "gl", "go", "goes", "going", "gone", "got", "gotten", "gr", "greetings", "gs", "gy", "h", "h2", "h3", "had", "hadn", "hadn't", "happens",
                                "hardly", "has", "hasn", "hasnt", "hasn't", "have", "haven", "haven't", "having", "he", "hed", "he'd", "he'll", "hello", "help", "hence",
                                "her", "here", "hereafter", "hereby", "herein", "heres", "here's", "hereupon", "hers", "herself", "hes", "he's", "hh", "hi", "hid", "him",
                                "himself", "his", "hither", "hj", "ho", "home", "hopefully", "how", "howbeit", "however", "how's", "hr", "hs", "http", "hu", "hundred", "hy",
                                "i", "i2", "i3", "i4", "i6", "i7", "i8", "ia", "ib", "ibid", "ic", "id", "i'd", "ie", "if", "ig", "ignored", "ih", "ii", "ij", "il", "i'll",
                                "im", "i'm", "immediate", "immediately", "importance", "important", "in", "inasmuch", "inc", "indeed", "index", "indicate", "indicated", "indicates",
                                "information", "inner", "insofar", "instead", "interest", "into", "invention", "inward", "io", "ip", "iq", "ir", "is", "isn", "isn't", "it", "itd", "it'd",
                                "it'll", "its", "it's", "itself", "iv", "i've", "ix", "iy", "iz", "j", "jj", "jr", "js", "jt", "ju", "just", "k", "ke", "keep", "keeps",
                                "kept", "kg", "kj", "km", "know", "known", "knows", "ko", "l", "l2", "la", "largely", "last", "lately", "later", "latter", "latterly",
                                "lb", "lc", "le", "least", "les", "less", "lest", "let", "lets", "let's", "lf", "like", "liked", "likely", "line", "little", "lj", "ll",
                                "ll", "ln", "lo", "look", "looking", "looks", "los", "lr", "ls", "lt", "ltd", "m", "m2", "ma", "made", "mainly", "make", "makes", "many", "may", "maybe", "me", "mean", "means", "meantime", "meanwhile", "merely", "mg", "might", "mightn", "mightn't", "mill", "million", "mine", "miss", "ml", "mn", "mo", "more", "moreover", "most", "mostly", "move", "mr", "mrs", "ms", "mt", "mu", "much", "mug", "must", "mustn", "mustn't", "my", "myself", "n", "n2", "na", "name", "namely", "nay", "nc", "nd", "ne", "near", "nearly", "necessarily", "necessary", "need", "needn", "needn't", "needs", "neither", "never", "nevertheless", "new", "next", "ng", "ni", "nine", "ninety", "nj", "nl", "nn", "no", "nobody", "non", "none", "nonetheless", "noone", "nor", "normally", "nos", "not", "noted", "nothing", "novel", "now", "nowhere", "nr", "ns", "nt", "ny", "o", "oa", "ob", "obtain", "obtained", "obviously", "oc", "od", "of", "off", "often", "og", "oh", "oi", "oj", "ok", "okay", "ol", "old", "om", "omitted", "on", "once", "one", "ones", "only", "onto", "oo", "op", "oq", "or", "ord", "os", "ot", "other", "others", "otherwise", "ou", "ought", "our", "ours", "ourselves", "out", "outside", "over", "overall", "ow", "owing", "own", "ox", "oz", "p", "p1", "p2", "p3", "page", "pagecount", "pages", "par", "part", "particular", "particularly", "pas", "past", "pc", "pd", "pe", "per", "perhaps", "pf", "ph", "pi", "pj", "pk", "pl", "placed", "please", "plus", "pm", "pn", "po", "poorly", "possible", "possibly", "potentially", "pp", "pq", "pr", "predominantly", "present", "presumably", "previously", "primarily", "probably", "promptly", "proud", "provides", "ps", "pt", "pu", "put", "py", "q", "qj", "qu", "que", "quickly", "quite", "qv", "r", "r2", "ra", "ran", "rather", "rc", "rd", "re", "readily", "really", "reasonably", "recent", "recently", "ref", "refs", "regarding", "regardless", "regards", "related", "relatively", "research", "research-articl", "respectively", "resulted", "resulting", "results", "rf", "rh", "ri", "right", "rj", "rl", "rm", "rn", "ro", "rq", "rr", "rs", "rt", "ru", "run", "rv", "ry", "s", "s2", "sa", "said", "same", "saw", "say", "saying", "says", "sc", "sd", "se", "sec", "second", "secondly", "section", "see", "seeing", "seem", "seemed", "seeming", "seems", "seen", "self", "selves", "sensible", "sent", "serious", "seriously", "seven", "several", "sf", "shall", "shan", "shan't", "she", "shed", "she'd", "she'll", "shes", "she's", "should", "shouldn", "shouldn't", "should've", "show", "showed", "shown", "showns", "shows", "si", "side", "significant", "significantly", "similar", "similarly", "since", "sincere", "six", "sixty", "sj", "sl", "slightly", "sm", "sn", "so", "some", "somebody", "somehow", "someone", "somethan", "something", "sometime", "sometimes", "somewhat", "somewhere", "soon", "sorry", "sp", "specifically", "specified", "specify", "specifying", "sq", "sr", "ss", "st", "still", "stop", "strongly", "sub", "substantially", "successfully", "such", "sufficiently", "suggest", "sup", "sure", "sy", "system", "sz", "t", "t1", "t2", "t3", "take", "taken", "taking", "tb", "tc", "td", "te", "tell", "ten", "tends", "tf", "th", "than", "thank", "thanks", "thanx", "that", "that'll", "thats", "that's", "that've", "the", "their", "theirs", "them", "themselves", "then", "thence", "there", "thereafter", "thereby", "thered", "therefore", "therein", "there'll", "thereof", "therere", "theres", "there's", "thereto", "thereupon", "there've", "these", "they", "theyd", "they'd", "they'll", "theyre", "they're", "they've", "thickv", "thin", "think", "third", "this", "thorough", "thoroughly", "those", "thou", "though", "thoughh", "thousand", "three", "throug", "through", "throughout", "thru", "thus", "ti", "til", "tip", "tj", "tl", "tm", "tn", "to", "together", "too", "took", "top", "toward", "towards", "tp", "tq", "tr", "tried", "tries", "truly", "try", "trying", "ts", "t's", "tt", "tv", "twelve", "twenty", "twice", "two", "tx", "u", "u201d", "ue", "ui", "uj", "uk", "um", "un", "under", "unfortunately", "unless", "unlike", "unlikely", "until", "unto", "uo", "up", "upon", "ups", "ur", "us", "use", "used", "useful", "usefully", "usefulness", "uses", "using", "usually", "ut", "v", "va", "value", "various", "vd", "ve", "ve", "very", "via", "viz", "vj", "vo", "vol", "vols", "volumtype", "vq", "vs", "vt", "vu", "w", "wa", "want", "wants", "was", "wasn", "wasnt", "wasn't", "way", "we", "wed", "we'd", "welcome", "well", "we'll", "well-b", "went", "were", "we're", "weren", "werent", "weren't", "we've", "what", "whatever", "what'll", "whats", "what's", "when", "whence", "whenever", "when's", "where", "whereafter", "whereas", "whereby", "wherein", "wheres", "where's", "whereupon", "wherever", "whether", "which", "while", "whim", "whither", "who", "whod", "whoever", "whole", "who'll", "whom", "whomever", "whos", "who's", "whose", "why", "why's", "wi", "widely", "will", "willing", "wish", "with", "within", "without", "wo", "won", "wonder", "wont", "won't", "words", "world", "would", "wouldn", "wouldnt", "wouldn't", "www", "x", "x1", "x2", "x3", "xf", "xi", "xj", "xk", "xl", "xn", "xo", "xs", "xt", "xv", "xx", "y", "y2", "yes", "yet", "yj", "yl", "you", "youd", "you'd", "you'll", "your", "youre", "you're", "yours", "yourself", "yourselves", "you've", "yr", "ys", "yt", "z", "zero", "zi", "zz"};

    for (string token : queryList)
    {
        string lower = convertToLowerCase(token);
        bool isStopWord = false;
        for (string sw : stopWords)
        {
            if (lower == sw)
            {
                isStopWord = true;
            }
        }

        if (!isStopWord)
        {
            apiCall += " " + token;
            apiCall += " " + to_string(freq(lower));
        }
    }
}

vector<vector<string>> generateInputVector()
{
    vector<vector<string>> input; //(query, N/P, weight)
    std::ifstream queryFile("query1.txt");
    if (queryFile)
    {
        string str;
        float runningWt = 1.00;
        while (getline(queryFile, str))
        {

            if (str.length()) // nonempty line
            {
                if (str.back() == '!')
                {
                    str.pop_back();
                    input.push_back({str, "P", "4.482"}); //e^1.5
                }
                else
                {
                    int wt = pow(e, runningWt);
                    input.push_back({str, "N", to_string(wt)});
                    runningWt -= 0.1;
                }
            }
            else
            {
                runningWt = 1; // reset runningWt for each new set of words
            }
        }
    }
    else
    {
        cout << "Error opening file\n";
    }

    // for (auto v : input)
    // {
    //     for (string s : v)
    //     {
    //         // cout << s << " ";
    //     }
    //     // cout << "\n";
    // }

    return input;
}

void QNA_tool::query(string question, string filename)
{
    vector<string> queryList = {};

    string separators = " .,-:!\"\'()?[];@";
    string word = "";
    for (int i = 0; i < question.size(); i++)
    {
        if (is_separator_v2(separators, question[i]))
        {
            if (word != "")
            {
                queryList.push_back(word);
            }
            word = "";
        }
        else
        {
            word += question[i];
        }
    }
    // vector<string> queryList = {"views", "partition", "Mahatama", "Gandhi", "?"};
    string apiCall = "python3 api_call_queryProc.py";
    generateQueryString(queryList, apiCall); // updates apiCall
    
    system(apiCall.c_str());

    // now query1.txt is generated, use it to generate input for para ranking
    vector<vector<string>> inputs = generateInputVector();
    // process now
    int k = 10;
    Node *res = get_top_k_para2(inputs, 10);
    string api_key = "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";
    query_llm(filename, res, k, api_key, question);
}

std::string QNA_tool::get_paragraph(int book_code, int page, int paragraph)
{

    cout << "Book_code: " << book_code << " Page: " << page << " Paragraph: " << paragraph << endl;

    std::string filename = "corpus/mahatma-gandhi-collected-works-volume-";
    filename += to_string(book_code);
    filename += ".txt";

    std::ifstream inputFile(filename);

    std::string tuple;
    std::string sentence;

    if (!inputFile.is_open())
    {
        std::cerr << "Error: Unable to open the input file " << filename << "." << std::endl;
        exit(1);
    }

    std::string res = "";

    while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence))
    {
        // Get a line in the sentence
        tuple += ')';

        int metadata[5];
        std::istringstream iss(tuple);

        // Temporary variables for parsing
        std::string token;

        // Ignore the first character (the opening parenthesis)
        iss.ignore(1);

        // Parse and convert the elements to integers
        int idx = 0;
        while (std::getline(iss, token, ','))
        {
            // Trim leading and trailing white spaces
            size_t start = token.find_first_not_of(" ");
            size_t end = token.find_last_not_of(" ");
            if (start != std::string::npos && end != std::string::npos)
            {
                token = token.substr(start, end - start + 1);
            }

            // Check if the element is a number or a string
            if (token[0] == '\'')
            {
                // Remove the single quotes and convert to integer
                int num = std::stoi(token.substr(1, token.length() - 2));
                metadata[idx] = num;
            }
            else
            {
                // Convert the element to integer
                int num = std::stoi(token);
                metadata[idx] = num;
            }
            idx++;
        }

        if (
            (metadata[0] == book_code) &&
            (metadata[1] == page) &&
            (metadata[2] == paragraph))
        {
            res += sentence;
        }
    }

    inputFile.close();
    return res;
}

void QNA_tool::query_llm(string filename, Node *root, int k, string API_KEY, string question)
{

    // first write the k paragraphs into different files

    Node *traverse = root;
    int num_paragraph = 0;

    while (num_paragraph < k)
    {
        assert(traverse != nullptr);
        string p_file = "paragraph_";
        p_file += to_string(num_paragraph);
        p_file += ".txt";
        // delete the file if it exists
        remove(p_file.c_str());
        ofstream outfile(p_file);
        string paragraph = get_paragraph(traverse->book_code, traverse->page, traverse->paragraph);
        assert(paragraph != "$I$N$V$A$L$I$D$");
        outfile << paragraph;
        outfile.close();
        traverse = traverse->right;
        num_paragraph++;
    }

    // write the query to query.txt
    ofstream outfile("query.txt");
    outfile << "These are the excerpts from Mahatma Gandhi's books.\nOn the basis of this, ";
    outfile << question;
    outfile<<"Begin with an introduction. Do not take help from any other sources and answer the query only on behalf of the given excerpts. Generate a 500 word output elaborating the answer properly. Also provide a conclusion at the end.";
    outfile.close();


    // python3 <filename> API_KEY num_paragraphs query.txt

    string command = "python3 ";
    command += filename;
    command += " ";
    command += API_KEY;
    command += " ";
    
    for(int i{0};i<k;i++){
        command +="paragraph_";
        command +=to_string(i);
        command +=".txt";
        command +=" ";
        
    }

    command += "query.txt";

    system(command.c_str());
    return;
}