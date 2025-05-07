#include <iostream>
#include <omp.h>
#include <ctime>
#include <cstdlib>
#include <climits>
using namespace std;

// Find the minimum value
void find_min(int *arr, int n) {
    int min_val = INT_MAX;

    double start_time = omp_get_wtime(); // Start time
    #pragma omp parallel for reduction(min:min_val)
    for (int i = 0; i < n; i++) {
        #pragma omp critical
        {
            cout << "Thread = " << omp_get_thread_num() << " and i = " << i << endl;
        }
        if (arr[i] < min_val) {
            min_val = arr[i];
        }
    }
    double end_time = omp_get_wtime(); // End time
    cout << "\nMinimum value = " << min_val << endl;
    cout << "Time for finding minimum: " << end_time - start_time << " seconds" << endl;
}

// Find the maximum value
void find_max(int *arr, int n) {
    int max_val = INT_MIN;

    double start_time = omp_get_wtime(); // Start time
    #pragma omp parallel for reduction(max:max_val)
    for (int i = 0; i < n; i++) {
        #pragma omp critical
        {
            cout << "Thread = " << omp_get_thread_num() << " and i = " << i << endl;
        }
        if (arr[i] > max_val) {
            max_val = arr[i];
        }
    }
    double end_time = omp_get_wtime(); // End time
    cout << "\nMaximum value = " << max_val << endl;
    cout << "Time for finding maximum: " << end_time - start_time << " seconds" << endl;
}

// Compute the sum of all elements
void compute_sum(int *arr, int n) {
    long long sum = 0;

    double start_time = omp_get_wtime(); // Start time
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < n; i++) {
        sum += arr[i];
        #pragma omp critical
        {
            cout << "Thread " << omp_get_thread_num() << " and i = " << i << endl;
        }
    }
    double end_time = omp_get_wtime(); // End time
    cout << "\nSum = " << sum << endl;
    cout << "Time for computing sum: " << end_time - start_time << " seconds" << endl;
}

// Compute the average of the array
void compute_average(int *arr, int n) {
    long long sum = 0;

    double start_time = omp_get_wtime(); // Start time
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < n; i++) {
        sum += arr[i];
        #pragma omp critical
        {
            cout << "Thread " << omp_get_thread_num() << " and i = " << i << endl;
        }
    }
    double end_time = omp_get_wtime(); // End time
    double avg = static_cast<double>(sum) / n;
    cout << "\nAverage = " << avg << endl;
    cout << "Time for computing average: " << end_time - start_time << " seconds" << endl;
}

int main() {
    omp_set_num_threads(4);
    srand(time(0)); // Seed once

    int n;
    cout << "Enter the number of elements in the array: ";
    cin >> n;

    // User decides the number of threads
    int numThreads;
    cout << "Enter number of threads to use: ";
    cin >> numThreads;

    omp_set_num_threads(numThreads); // Set number of threads for OpenMP

    srand(time(0)); // Seed once


    int* arr = new int[n]; // Allocate array dynamically

    for (int i = 0; i < n; ++i) {
        arr[i] = rand() % 100; // Random values between 0 and 99
    }

    cout << "\nArray elements: ";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    // Perform operations with user-specified threads and measure time
    find_min(arr, n);
    find_max(arr, n);
    compute_sum(arr, n);    // Calculate sum separately
    compute_average(arr, n); // Calculate average separately

    delete[] arr; // Clean up
    return 0;
}

