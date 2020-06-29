#include <cmath>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

std::mt19937 gen(std::random_device{}());

template <typename T>
size_t merge(std::vector<T>& A, size_t p, size_t q, size_t r) {
    size_t i = p;
    size_t j = q;
    size_t count = 0;
    std::vector<T> B (r - p);
    for (size_t k = 0; k < B.size(); k++) {
        if (A[i] < A[j] || j >= r) {
            B[k] = A[i];
            i++;
        } else {
            B[k] = A[j];
            j++;
            count += (q - i);
        }
    }
    std::copy(A.begin() + p, A.begin() + r, B.begin());
    return count;
}

template <typename T>
size_t inversionsHelper(std::vector<T>& A, size_t p, size_t r) {
    if (p < r) {
        size_t q = p + (r - p) / 2;
        size_t count = 0;
        count += inversionsHelper(A, p, q);
        count += inversionsHelper(A, q + 1, r);
        count += merge(A, p, q, r);
        return count;
    }
    return 0;
}

template <typename T>
size_t inversions(std::vector<T>& A) {
    return inversionsHelper(A, 0, A.size() - 1);
}

int main() {
    constexpr size_t N = 1000;
    constexpr size_t trials = 1000;

    std::vector<int> hats(N);
    std::iota(hats.begin(), hats.end(), 1);
    size_t count = 0;
    for (size_t i = 0; i < trials; i++) {
        auto shuffled_hats = hats;
        std::shuffle(shuffled_hats.begin(), shuffled_hats.end(), gen);
        count += inversions(shuffled_hats);
    }
    std::cout << "Average inversion count : " << static_cast<double>(count) / static_cast<double>(trials) << '\n';
    std::cout << "N * (N - 1) / 4 : " << N * (N - 1) / 4.0 << '\n';

}