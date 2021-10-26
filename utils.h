#ifndef CACHEINFO_UTILS_H
#define CACHEINFO_UTILS_H

#include <string>

struct ArrayFabric {
    virtual std::pair<char*, char**> createArray(int32_t size, int32_t stride) = 0;
};

struct PreviousStep: ArrayFabric {
    std::pair<char*, char**> createArray(int32_t size, int32_t stride) override;
};

struct RandomStep: ArrayFabric {
    std::pair<char*, char**> createArray(int32_t size, int32_t stride) override;
};

long long loads(int32_t size, int32_t stride, int32_t factor, int32_t iterCount, ArrayFabric &fabric, bool isConstIterations, std::string unit);

void printWithFactor(const std::map<int32_t, long long> &m);

#endif
