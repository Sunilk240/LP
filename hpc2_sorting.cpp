#include <iostream>
#include <cstdlib>
#include <omp.h>

using namespace std;

// ====================== Bubble Sort ==========================

// Sequential Bubble Sort
void sequentialBubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; ++i)
        for (int j = 0; j < n - i - 1; ++j)
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
}

// Parallel Bubble Sort using Odd-Even Transposition
void parallelBubbleSort(int arr[], int n) {
    for (int i = 0; i < n; ++i) {
        int phase = i % 2;
        #pragma omp parallel for
        for (int j = phase; j < n - 1; j += 2) {
            // int tid = omp_get_thread_num();
            // cout << "Thread " << tid << " working on index " << j << endl;
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
        }
    }
}

// ===================== Merge Sort Helpers ======================

// Merge utility for both versions
void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    int *L = new int[n1];
    int *R = new int[n2];

    for (int i = 0; i < n1; ++i)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2)
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

// ====================== Merge Sort ============================

// Sequential Merge Sort
void sequentialMergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        sequentialMergeSort(arr, l, m);
        sequentialMergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// // Parallel Merge Sort
// void parallelMergeSort(int arr[], int l, int r) {
//     if (l < r) {
//         int m = l + (r - l) / 2;

//         #pragma omp parallel sections
//         {
//             #pragma omp section
//             {
//                 // int tid = omp_get_thread_num();
//                 // cout << "Thread " << tid << " sorting left half\n";
//                 parallelMergeSort(arr, l, m);
//             }

//             #pragma omp section
//             {
//                 // int tid = omp_get_thread_num();
//                 // cout << "Thread " << tid << " sorting right half\n";
//                 parallelMergeSort(arr, m + 1, r);
//             }
//         }

//         merge(arr, l, m, r);
//     }
// }

void parallelMergeSort(int arr[], int l, int r, int depth = 0) {
    if (l < r) {
        int m = l + (r - l) / 2;

        if (depth <= 3) {  // Limit depth to avoid thread explosion
            #pragma omp parallel sections
            {
                #pragma omp section
                parallelMergeSort(arr, l, m, depth + 1);
                #pragma omp section
                parallelMergeSort(arr, m + 1, r, depth + 1);
            }
        } else {
            sequentialMergeSort(arr, l, m);
            sequentialMergeSort(arr, m + 1, r);
        }

        merge(arr, l, m, r);
    }
}


// print the array
void printArray(int arr[], int n, const string& label) {
    cout << label << ": ";
    for (int i = 0; i < n; ++i)
        cout << arr[i] << " ";
    cout << endl;
}


// ====================== Main Driver ===========================

int main() {
    int n;
    cout << "Enter the size of the array: ";
    cin >> n;

    // Original data
    int *original = new int[n];
    for (int i = 0; i < n; ++i)
        original[i] = rand() % 1000;

    // Copies
    int *arrBubbleSeq = new int[n];
    int *arrBubblePar = new int[n];
    int *arrMergeSeq  = new int[n];
    int *arrMergePar  = new int[n];

    for (int i = 0; i < n; ++i)
        arrBubbleSeq[i] = arrBubblePar[i] = arrMergeSeq[i] = arrMergePar[i] = original[i];

    // printArray(original, n, "Original Array");
    cout<<endl;

    double start, end;

    int numThreads;
    cout << "Enter number of threads to use: ";
    cin >> numThreads;
    omp_set_num_threads(numThreads);
    cout << "Using " << numThreads << " threads for parallel operations.\n";


    // Sequential Bubble Sort
    start = omp_get_wtime();
    sequentialBubbleSort(arrBubbleSeq, n);
    end = omp_get_wtime();
    double time_seq_bubble = end - start;
    // printArray(arrBubbleSeq, n, "Sorted (Sequential Bubble)");

    // Parallel Bubble Sort
    start = omp_get_wtime();
    parallelBubbleSort(arrBubblePar, n);
    end = omp_get_wtime();
    double time_par_bubble = end - start;
    // printArray(arrBubblePar, n, "Sorted (Parallel Bubble)");

    // Sequential Merge Sort
    start = omp_get_wtime();
    sequentialMergeSort(arrMergeSeq, 0, n - 1);
    end = omp_get_wtime();
    double time_seq_merge = end - start;
    // printArray(arrMergeSeq, n, "Sorted (Sequential Merge)");

    // Parallel Merge Sort
    start = omp_get_wtime();
    parallelMergeSort(arrMergePar, 0, n - 1);
    end = omp_get_wtime();
    double time_par_merge = end - start;
    // printArray(arrMergePar, n, "Sorted (Parallel Merge)");

    // Results
    cout << "\nPerformance Comparison (in seconds):\n";
    cout << "Sequential Bubble Sort : " << time_seq_bubble << endl;
    cout << "Parallel Bubble Sort   : " << time_par_bubble << endl;
    cout << "Sequential Merge Sort  : " << time_seq_merge << endl;
    cout << "Parallel Merge Sort    : " << time_par_merge << endl;

    delete[] original;
    delete[] arrBubbleSeq;
    delete[] arrBubblePar;
    delete[] arrMergeSeq;
    delete[] arrMergePar;

    return 0;
}
