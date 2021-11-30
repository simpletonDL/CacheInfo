#include <iostream>
#include <map>
#include <memory>
#include "utils.h"

using namespace std;

int main() {
    int32_t startStride = 1 << 3;
    int32_t endStride = 1 << 12;
    RandomStep fabric;

    int32_t arrSize = 32 * 1024 * 1024;

    map<int32_t, long long> results;
    for (int stride = startStride; stride <= endStride; stride <<= 1) {
        cout << "Computing stride " << stride << "\n";
        results[stride] = loads(arrSize, stride, 10, 10, fabric, false, "us");
    }
    printWithFactor(results);
    return 0;
}
