#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <random>
#include <numeric>
#include <vector>

std::mt19937 gen(std::random_device{}());

template <typename T>
size_t partition(std::vector<T>& A, size_t p, size_t r) {
    T x = A[r];
    size_t i = p;
    for (size_t j = p; j < r; j++) {
        if (A[j] <= x) {
            std::swap(A[i], A[j]);
            i++;
        }
    }
    std::swap(A[i], A[r]);
    return i;
}

template <typename T>
void quickSort(std::vector<T>& A, size_t p, size_t r) {
    if (p < r && r < A.size()) {
        size_t q = partition(A, p, r);
        quickSort(A, p, q - 1);
        quickSort(A, q + 1, r);
    }
}

constexpr size_t SORT_CRITERION = 15;

template <typename T>
void truncatedQuickSort(std::vector<T>& A, size_t p, size_t r) {
    if (p < r && r < A.size() && (r - p) > SORT_CRITERION) {
        size_t q = partition(A, p, r);
        truncatedQuickSort(A, p, q - 1);
        truncatedQuickSort(A, q + 1, r);
    }
}

template <typename T, typename Comp = std::greater<T>>
void insertionSort(std::vector<T>& A, Comp comp = Comp()) {
    for (size_t j = 1; j < A.size(); j++) {
        T key = A[j];
        size_t i = j - 1;
        while (i < A.size() && comp(A[i], key)) {
            A[i + 1] = A[i];
            i--;
        }
        A[i + 1] = key;
    }
}

template <typename T>
void mixedSort(std::vector<T>& A) {
    truncatedQuickSort(A, 0, A.size() - 1);
    insertionSort(A);
}

int main() {
    constexpr size_t N = 10'000;
    std::vector<int> v (N);
    std::iota(v.begin(), v.end(), 0);

    constexpr size_t TRIALS = 1'000;

    std::chrono::microseconds DT(0), DT2(0), DT3(0);
    for (size_t t = 0; t < TRIALS; t++) {
        auto u = v;
        std::shuffle(u.begin(), u.end(), gen);
        auto t1 = std::chrono::steady_clock::now();
        quickSort(u, 0, u.size() - 1);
        auto t2 = std::chrono::steady_clock::now();
        auto dt = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
        DT += dt;

        std::shuffle(u.begin(), u.end(), gen);
        auto t3 = std::chrono::steady_clock::now();
        mixedSort(u);
        auto t4 = std::chrono::steady_clock::now();
        auto dt2 = std::chrono::duration_cast<std::chrono::microseconds>(t4 - t3);
        DT2 += dt2;

        std::shuffle(u.begin(), u.end(), gen);
        auto t5 = std::chrono::steady_clock::now();
        std::sort(u.begin(), u.end());
        auto t6 = std::chrono::steady_clock::now();
        auto dt3 = std::chrono::duration_cast<std::chrono::microseconds>(t6 - t5);

        DT3 += dt3;
    }
    std::cout << "Average performance of quicksort on " << N << " elements : " << DT.count() / TRIALS << "ms\n";
    std::cout << "Average performance of truncated quicksort + insertion sort on " << N << " elements : " << DT2.count() / TRIALS << "ms\n";
    std::cout << "Average performance of std::sort on " << N << " elements : " << DT3.count() / TRIALS << "ms\n";

}