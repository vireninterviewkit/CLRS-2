#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

std::mt19937 gen(std::random_device{}());

template <typename T>
std::pair<T, T> intersection(std::vector<std::pair<T, T>>& A, size_t p, size_t r) {
    std::uniform_int_distribution<> dist (p, r);
    size_t j = dist(gen);
    std::swap(A[j], A[r]);
    auto [b, e] = A[r];
    for (size_t i = p; i < r; i++) {
        if (A[i].first <= e && A[i].second >= b) {
            b = std::max(A[i].first, b);
            e = std::min(A[i].second, e);
        }
    }
    return {b, e};
}

template <typename T>
size_t partitionRight(std::vector<std::pair<T, T>>& A, const T& b, size_t p, size_t r) {
    size_t i = p - 1;
    for (size_t j = p; j < r; j++) {
        if (A[j].first <= b) {
            i++;
            std::swap(A[i], A[j]);
        }
    }
    std::swap(A[i + 1], A[r]);
    return i + 1;
}

template <typename T>
size_t partitionLeft(std::vector<std::pair<T, T>>& A, const T& e, size_t p, size_t r) {
    size_t i = p - 1;
    for (size_t j = p; j < r; j++) {
        if (A[j].second < e) {
            i++;
            std::swap(A[i], A[j]);
        }
    }
    std::swap(A[i + 1], A[r]);
    return i + 1;
}

template <typename T>
void fuzzySort(std::vector<std::pair<T, T>>& A, size_t p, size_t r) {
    if (p < r && r < A.size()) {
        auto [b, e] = intersection(A, p, r);
        auto t = partitionRight(A, b, p, r);
        auto q = partitionLeft(A, e, p, t);
        fuzzySort(A, p, q - 1);
        fuzzySort(A, t + 1, r);
    }
}

int main() {
    std::vector<std::pair<int, int>> v;
    constexpr size_t NUM_ELEMS = 100;
    std::uniform_int_distribution<> dist (0, 1000);
    for (size_t i = 0; i < NUM_ELEMS; i++) {
        auto a = dist(gen);
        auto b = dist(gen);
        if (a <= b) {
            v.emplace_back(a, b);
        } else {
            v.emplace_back(b, a);
        }
    }
    fuzzySort(v, 0, v.size() - 1);
    int c = 0;
    for (const auto& it : v) {
        c = std::max(c, it.first);
        if (c > it.second) {
            throw std::runtime_error("the algorithm is wrong!");
        }
        std::cout << it.first << ", " << it.second << " : " << c << '\n';
    }

}