#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

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
            count++;
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
    std::vector<int> v {5, 4, 3, 2, 1};
    std::cout << inversions(v);


}