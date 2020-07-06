#include <algorithm>
#include <cassert>
#include <vector>

std::vector<size_t> countingSort(std::vector<size_t>& A, size_t k) {
    assert(*std::max_element(A.begin(), A.end()) <= k);
    std::vector<size_t> B (A.size());
    std::vector<size_t> C (k + 1);
    for (auto n : A) {
        C[n]++;
    }
    for (size_t j = 1; j <= k; j++) {
        C[j] += C[j - 1];
    }
    for (auto n : A) {
        B[C[n]] = n;
        C[n]--;
    }
    return B;
}

int main() {
    std::vector<size_t> v {3, 2, 6, 1, 5, 4};
    v = countingSort(v, 6);
    assert(std::is_sorted(v.begin(), v.end()));
}