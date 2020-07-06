#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

std::mt19937 gen(std::random_device{}());

void radixSort(std::vector<size_t>& A, size_t d) {
    assert(*std::max_element(A.begin(), A.end()) < std::pow(10u, d));
    for (size_t i = 0; i < d; i++) {
        auto comp = [&i](size_t a, size_t b) {
            return ((a / static_cast<size_t>(std::pow(10u, i))) % 10) <
                   ((b / static_cast<size_t>(std::pow(10u, i))) % 10);
        };
        std::stable_sort(A.begin(), A.end(), comp);
    }
}

int main() {
    constexpr size_t N = 50'000;
    std::vector<size_t> A (N);
    std::uniform_int_distribution<> dist(0, 99);
    for (auto& n : A) {
        n = dist(gen);
    }
    constexpr size_t TRIALS = 1'000;
    std::chrono::milliseconds DT1 (0), DT2(0);
    for (size_t t = 0; t < TRIALS; t++) {
        std::shuffle(A.begin(), A.end(), gen);
        auto t1 = std::chrono::steady_clock::now();
        std::sort(A.begin(), A.end());
        auto t2 = std::chrono::steady_clock::now();
        auto dt1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
        DT1 += dt1;
        assert(std::is_sorted(A.begin(), A.end()));
        std::shuffle(A.begin(), A.end(), gen);
        auto t3 = std::chrono::steady_clock::now();
        radixSort(A, 2);
        auto t4 = std::chrono::steady_clock::now();
        auto dt2 = std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3);
        DT2 += dt2;
        assert(std::is_sorted(A.begin(), A.end()));
    }

    std::cout << "Sorting " << N << " elements in range of [0, 99] using std::sort : " << DT1.count() / TRIALS << "ms\n";
    std::cout << "Sorting " << N << " elements in range of [0, 99] using radix sort : " << DT2.count() / TRIALS << "ms\n";


}