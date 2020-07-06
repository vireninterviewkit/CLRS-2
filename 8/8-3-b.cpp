#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <iostream>
#include <random>
#include <string>
#include <vector>

std::mt19937 gen(std::random_device{}());

static const std::string chars = "abcdefghijklmnopqrstuvwxyz";

void bucketSort(std::vector<std::string>& A, size_t pos) {
    if (pos == 5) {
        return;
    }
    std::vector<std::vector<std::string>> B (27);
    for (const auto& str : A) {
        if (str.size() <= pos) {
            B[0].push_back(str);
        } else {
            B[1 + str[pos] - 'a'].push_back(str);
        }
    }
    for (size_t i = 0; i < 27; i++) {
        bucketSort(B[i], pos + 1);
    }
    A.clear();
    for (auto& v : B) {
        A.insert(A.end(), std::make_move_iterator(v.begin()), std::make_move_iterator(v.end()));
    }
}

std::string make_random_string() {
    std::string res;
    std::uniform_int_distribution<> char_dist(0, 25);
    std::uniform_int_distribution<> length_dist(1, 5);
    size_t l = length_dist(gen);
    for (size_t i = 0; i < l; i++) {
        res += chars[char_dist(gen)];
    }
    return res;
}

int main() {
    constexpr size_t N = 100;
    std::vector<std::string> A;
    for (size_t i = 0; i < N; i++) {
        A.push_back(make_random_string());
    }
    constexpr size_t TRIALS = 20;
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
        bucketSort(A, 0);
        auto t4 = std::chrono::steady_clock::now();
        auto dt2 = std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3);
        DT2 += dt2;
        assert(std::is_sorted(A.begin(), A.end()));
    }

    std::cout << "Sorting " << N << " elements in using std::sort : " << DT1.count() / TRIALS << "ms\n";
    std::cout << "Sorting " << N << " elements in using recursive bucket sort : " << DT2.count() / TRIALS << "ms\n";


}