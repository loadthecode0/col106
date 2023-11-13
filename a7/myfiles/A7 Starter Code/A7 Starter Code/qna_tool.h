#include <iostream>
#include <fstream>
#include "Node.h"

// my includes

#include "dict.h"
#include "search.h"

using namespace std;

class Master_para_node
{
public:
public:
    int book_code;
    int page;
    int paragraph;
    long double para_score;

    Master_para_node();
    Master_para_node(int b_code, int pg, int para, long double p_score);
    bool operator>(const Master_para_node &other) const;
};

class BinaryHeap
{

public:
    std::vector<Master_para_node *> heap;
    void swap(Master_para_node *&a, Master_para_node *&b)
    {
        Master_para_node *temp = a;
        a = b;
        b = temp;
    }

    int getParentIndex(int i)
    {
        return (i - 1) / 2;
    }

    int getLeftChildIndex(int i)
    {
        return 2 * i + 1;
    }

    int getRightChildIndex(int i)
    {
        return 2 * i + 2;
    }

    void insert(Master_para_node *val)
    {
        heap.push_back(val); // satisfies the structural prop
        heapifyUp(heap.size() - 1);
    }

    void heapifyUp(int index)
    {
        if (index == 0)
            return; // base condition for termination of a recursive invocation of the fnc

        int parentIndex = getParentIndex(index);

        if (heap[parentIndex]->para_score > heap[index]->para_score)
        {
            swap(heap[parentIndex], heap[index]);
            heapifyUp(parentIndex);
        }
    }

    void heapifyDown(int index)
    {
        int leftChild = getLeftChildIndex(index);
        int rightChild = getRightChildIndex(index);

        if (leftChild >= heap.size())
            return; // No children

        int minIndex = index;

        if (heap[minIndex]->para_score > heap[leftChild]->para_score)
        {
            minIndex = leftChild;
        }

        if (rightChild < heap.size() && heap[minIndex]->para_score > heap[rightChild]->para_score)
        {
            minIndex = rightChild;
        }

        if (minIndex != index)
        {
            swap(heap[minIndex], heap[index]);
            heapifyDown(minIndex);
        }
    }

    void heapifyDown(int n, int index)
    {
        int leftChild = getLeftChildIndex(index);
        int rightChild = getRightChildIndex(index);

        int minIndex = index;

        if (leftChild < n && heap[minIndex]->para_score > heap[leftChild]->para_score)
        {
            minIndex = leftChild;
        }

        if (rightChild < n && heap[minIndex]->para_score > heap[rightChild]->para_score)
        {
            minIndex = rightChild;
        }

        if (minIndex != index)
        {
            swap(heap[minIndex], heap[index]);
            heapifyDown(n, minIndex);
        }
    }
    // void heapifyDownMax(int index)
    // {
    //     int leftChild = getLeftChildIndex(index);
    //     int rightChild = getRightChildIndex(index);

    //     int maxIndex = index;

    //     if (leftChild < heap.size() && heap[maxIndex] < heap[leftChild])
    //     {
    //         maxIndex = leftChild;
    //     }

    //     if (rightChild < heap.size() && heap[maxIndex] < heap[rightChild])
    //     {
    //         maxIndex = rightChild;
    //     }

    //     if (maxIndex != index)
    //     {
    //         swap(heap[maxIndex], heap[index]);
    //         heapifyDownMax(maxIndex);
    //     }
    // }

    Master_para_node *minElem()
    {
        return heap[0];
    }

    void deleteMin()
    {
        if (heap.empty())
        {
            std::cout << "Heap is empty!" << std::endl;
            return;
        }

        heap[0] = heap.back();
        heap.pop_back();

        heapifyDown(0);
    }

    bool is_empty()
    {
        return heap.empty();
    }

    void buildHeap(const std::vector<Master_para_node *> &arr, int size)
    {

        for (int i = 0; i < size; i++)
            heap.push_back(arr[i]);

        int n = size;

        for (int i = n / 2 - 1; i >= 0; i--)
        {
            heapifyDown(i);
        }
    }

    void heapSort()
    {
        int n = heap.size();
        // Extract elements from heap one by one
        for (int i = n - 1; i > 0; i--)
        {
            // Move the root to the end
            std::swap(heap[0], heap[i]);

            // Heapify the reduced heap
            heapifyDown(i, 0);
        }
    }

    void printHeap()
    {
        for (const auto &elem : heap)
        {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }

    Master_para_node *findKthLargest(std::vector<Master_para_node *> &arr, int k)
    {

        for (int i = k; i < arr.size(); i++)
        {
            if (arr[k] > heap[0])
            {
                deleteMin();
                printHeap(); // debug func
                insert(arr[k]);
            }
        }
        return heap[0];
    }
};

class QNA_tool
{

private:
    // You are free to change the implementation of this function
    void query_llm(string filename, Node *root, int k, string API_KEY, string question);
    // filename is the python file which will call ChatGPT API
    // root is the head of the linked list of paragraphs
    // k is the number of paragraphs (or the number of nodes in linked list)
    // API_KEY is the API key for ChatGPT
    // question is the question asked by the user

    // You can add attributes/helper functions here

    // vector<vector<vector<long long int>>> ScoreStore;
    // vector<vector<vector<bool>>> visited_paras;
    // vector<para_nodes *> to_be_sorted;

public:
    Dict *personal_dictionary;
    Dict *global_dictionary;

    /* Please do not touch the attributes and
    functions within the guard lines placed below  */
    /* ------------------------------------------- */

    QNA_tool();  // Constructor
    ~QNA_tool(); // Destructor

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);
    // This function is similar to the functions in dict and search
    // The corpus will be provided to you via this function
    // It will be called for each sentence in the corpus

    Node *get_top_k_para(string question, int k);
    // This function takes in a question, preprocess it
    // And returns a list of paragraphs which contain the question
    // In each Node, you must set: book_code, page, paragraph (other parameters won't be checked)

    std::string get_paragraph(int book_code, int page, int paragraph);
    // Given the book_code, page number, and the paragraph number, returns the string paragraph.
    // Searches through the corpus.

    Node *query(string question, string filename);
    // This function takes in a question and a filename.
    // It should write the final answer to the specified filename.

    /* -----------------------------------------*/
    /* Please do not touch the code above this line */

    // You can add attributes/helper functions here
    Node *get_top_k_para2(vector<vector<string>> question, int k); /// need to be changed to string
    long long int freq(string &token);
    void generateQueryString(vector<string> queryList, string &apiCall);
};