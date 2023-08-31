#include <bits/stdc++.h>
using namespace std;

void swapElts(float* a, float* b)
{
    float temp = *a;
    *a = *b;
    *b = temp;
}

void quickSort(vector<float> &v, int start, int end) {
    if (start < end) {
        int n = end - start + 1;
        int pivotIndex = ((rand()) % (end - start + 1)) + start;
        int pivot = v[pivotIndex];

        //swap pivot with last
        swapElts(&v[pivotIndex], &v[end]);

        int i = start;
        while (i<end && v[i] <= pivot) {
            i++;
        } //ends up at first element greater than pivot elt

        if (i == end) { //all elts other than pivot are less than it 
            quickSort(v, start, end-1);
        } else {
            for (int j = i+1; j<=end; j++) {
                if (v[j] <= pivot){
                    swapElts(&v[i], &v[j]);
                    i++; //now filled by smaller elements till index i-1
                }
            }

            //swap i+1 and pivot(last)
            swapElts(&v[i], &v[end]);
            //called recursion for partition
            quickSort(v, start, i-2);
            quickSort(v, i, end);

        }
    }  
}

int main()
{
    vector<float> vec = {232, 124, 498, 776, 229, 3864, -2537, 3284, 2121, 23, 378, 3663};
    quickSort(vec, 0, 11);
    for (int i =0; i<12; i++) {
        cout << vec[i] << " ";
    }
    return 0;
}