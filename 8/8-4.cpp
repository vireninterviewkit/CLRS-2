#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

std::mt19937 gen(std::random_device{}());

std::vector<std::pair<int, int>> jugs;

void matchJugs(std::vector<int>& R, std::vector<int>& B) {
    assert(R.size() == B.size());
    if (R.empty()) {
        return;
    } else if (R.size() == 1) {
        jugs.emplace_back(R[0], B[0]);
    } else {
        std::uniform_int_distribution<> dist(0, R.size() - 1);
        int r = R[dist(gen)];
        std::vector<int> B_s, B_l, R_s, R_l;
        for (auto& b : B) {
            if (b == r) {
                jugs.emplace_back(r, b);
            } else if (b > r) {
                B_l.push_back(b);
            } else {
                B_s.push_back(b);
            }
        }
        auto b = jugs.back().second;
        for (auto t : R) {
            if (t == b) {
                continue;
            } else if (t > b) {
                R_l.push_back(t);
            } else {
                R_s.push_back(t);
            }
        }
        R.clear();
        B.clear();
        matchJugs(R_l, B_l);
        matchJugs(R_s, B_s);
    }
}

int main() {
    constexpr size_t N = 1'000;

    std::vector<int> R (N), B (N);
    std::iota(R.begin(), R.end(), 0);
    std::iota(B.begin(), B.end(), 0);
    std::shuffle(R.begin(), R.end(), gen);
    std::shuffle(B.begin(), B.end(), gen);
    matchJugs(R, B);
    assert(std::all_of(jugs.begin(), jugs.end(), [](auto& p) {return p.first == p.second;}));

}