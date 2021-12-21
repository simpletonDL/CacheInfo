#include <iostream>
#include <map>
#include <vector>
#include <memory>
#include "utils.h"

using namespace std;

int main() {
    // Нужно вставить свои константы :D
    const int32_t L1_SIZE = 32 * KB;
    const int32_t L1_ASSOC = 8;


    const int32_t MAX_STEP = 256;
    const int32_t MAX_READ_COUNT = 1024;
    const double JUMP_PREDICTOR = 1.3;

    RandomStep policity;

    // step -> [(readCount, time)]
    map<int32_t, vector<pair<int32_t, long long>>> times;

    for (int32_t step = 1; step <= MAX_STEP; step <<= 1) {
        cout << "step: " << step << "\n";
        for (int32_t readCount = 1; readCount <= MAX_READ_COUNT; readCount <<=1) {
            long long t = loadsForCacheLine(step, readCount, L1_SIZE / L1_ASSOC, policity, "ns");
            times[step].emplace_back(readCount, t);
            cout << "\treadCount=" << readCount << ": " << t << "\n";
        }
    }

    // step -> first jump, when readCount reads
    cout << "JUMPS:\n";
    map<int32_t, int32_t> s;
    for (auto it: times) {
        int32_t step = it.first;
        vector<pair<int32_t, long long>> ps = it.second;
        for (int i = 1; i < ps.size(); ++i) {
            auto curr_time = (double) ps[i].second;
            auto prev_time = (double) ps[i-1].second;
            if (curr_time / prev_time > JUMP_PREDICTOR) {
                s[step] = ps[i].first;
                break;
            }
        }
        cout << step << ": " << s[step] << "\n";
    }

    int32_t prevJump = s.begin()->second;
    int32_t ans = -1;
    for (auto it: s) {
        if (it.second != prevJump) {
            ans = it.first;
            break;
        }
        prevJump = it.second;
    }
    cout << "Cache Line: " << ans * L1_ASSOC;

    return 0;
}
