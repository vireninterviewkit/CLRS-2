#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <utility>
#include <vector>

size_t parent(size_t i) {
    return i / 2;
}

size_t left(size_t i) {
    return 2 * i + 1;
}

size_t right(size_t i) {
    return 2 * i + 2;
}

template <typename T>
void maxHeapifyIterative(std::vector<T>& A) {
    size_t i = 0;
    while (i <= A.size() / 2) {
        auto l = left(i);
        auto r = right(i);
        size_t largest = i;
        if (l < A.size() && A[l] > A[i]) {
            largest = l;
        }
        if (r < A.size() && A[r] > A[largest]) {
            largest = r;
        }
        if (largest != i) {
            std::swap(A[i], A[largest]);
            i = largest;
        } else {
            return;
        }
    }
}


int main() {
    std::vector<int> v {3, 1, 4, 1, 5, 9};
    std::make_heap(v.begin() + 1, v.end());
    maxHeapifyIterative(v);
    assert(std::is_heap(v.begin(), v.end()));

}