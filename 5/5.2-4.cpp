#include <cmath>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

std::mt19937 gen(std::random_device{}());

int main() {
    constexpr size_t N = 1000;
    constexpr size_t trials = 1000;

    std::vector<int> hats(N);
    std::iota(hats.begin(), hats.end(), 1);
    size_t count = 0;
    for (size_t i = 0; i < trials; i++) {
        auto shuffled_hats = hats;
        std::shuffle(shuffled_hats.begin(), shuffled_hats.end(), gen);
        for (size_t j = 0; j < N; j++) {
            if (shuffled_hats[j] == hats[j]) {
                count++;
            }
        }
    }
    std::cout << "Average matched count : " << static_cast<double>(count) / static_cast<double>(trials) << '\n';

}