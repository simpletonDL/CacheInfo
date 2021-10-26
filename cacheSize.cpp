#include <iostream>
#include <map>
#include "utils.h"

using namespace std;

int main() {
    int32_t startSize = 64;
    int32_t endSize = 64 * 1024 * 1024;
    int32_t cacheLineSize = 64;
    RandomStep fabric;

    map<int32_t, long long> result;
    for (int size = startSize; size < endSize; size <<= 1) {
        result[size] = loads(size, cacheLineSize, 10, 10, fabric, false, "ns");
    }
    printWithFactor(result);
}