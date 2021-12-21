#include <iostream>
#include "utils.h"
#include <map>
#include <vector>
#include <fstream>

using namespace std;

class F0 {
public:
    inline static void run() {
        cout << 1;
        cout << 2;
    }
};

// L1: 8-assoc    |  32 KB   |
// L2: 4-assoc    | 256 KB   |
// L3: 16-assoc   |  16 MB   |

int main() {
    int MAX_READ_COUNT = 34;
    int FACTOR = 1000000;

    map<int32_t, vector<long long>> results;
    RandomStep policity;

    for (int32_t stride = 8 * MB; stride >= 16; stride >>= 1) {
        cout << "Computing stride " <<  stride << ":\n";
        for (int readCount = 1; readCount <= MAX_READ_COUNT; readCount++) {
            auto time = loadsFixed(stride, readCount, FACTOR, policity, "ns");
            cout << readCount << ": " << time << "\n";
            results[stride].push_back(time);
        }
        cout << "\n";
    }

    ofstream out("output.csv");
    out << ":D,";
    for (auto it = results.begin(); it != results.end();) {
        out << it->first;
        if (++it != results.end()) {
            out << ",";
        }
    }
    out << "\n";
    for (int i = 0; i < MAX_READ_COUNT; i++) {
        out << i + 1 << ",";
        for (auto it = results.begin(); it != results.end();) {
            out << it->second[i];
            if (++it != results.end()) {
                out << ",";
            }
        }
        out << "\n";
    }
}
