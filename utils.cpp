#include <iostream>
#include <map>
#include <random>
#include "utils.h"

using namespace std;

pair<char*, char**> PreviousStep::createArray(int32_t size, int32_t stride) {
    char *array = new char[size];
    char **p = nullptr;

    // Initialization
    int i;
    for (i = stride; i < size; i += stride);
    i -= stride;
    for (; i >= 0; i -= stride) {
        char *next;

        p = (char **) &(array[i]);
        if (i < stride) {
            next = &array[size - stride];
        } else {
            next = &array[i - stride];
        }
        *p = next;
    }
    return make_pair(array, p);
}

void myShuffle(vector<int32_t> &v) {
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(v.begin(), v.end(), g);
}

pair<char*, char**> RandomStep::createArray(int32_t size, int32_t stride) {
    char *array = new char[size];
    char **p = nullptr;

    vector<int32_t> ids;
    for (int i = 0; i < size; i += stride) {
        ids.push_back(i);
    }
    myShuffle(ids);

    p = (char **) &(array[ids.front()]);
    *p = &(array[ids.back()]);

    for (int i = 1; i < ids.size(); i++) {
        int32_t cur = ids[i];
        int32_t prev = ids[i-1];

        p = (char **) &(array[cur]);
        *p = &array[prev];
    }

    // Initialization
    int i;
    for (i = stride; i < size; i += stride);
    i -= stride;
    for (; i >= 0; i -= stride) {
        char *next;

        p = (char **) &(array[i]);
        if (i < stride) {
            next = &array[size - stride];
        } else {
            next = &array[i - stride];
        }
        *p = next;
    }
    return make_pair(array, p);
}

void printWithFactor(const map<int32_t, long long> &m) {
    bool first = true;
    long long last;
    for (auto p: m) {
        if (first) {
            first = false;
            last = p.second;
            continue;
        } else {
            cout << p.first << ": " << p.second << " " << (double) p.second / (double) last << "\n";
            last = p.second;
        }
    }
}

long long loads(int32_t size, int32_t stride, int32_t factor, int32_t iterCount,
                ArrayFabric &fabric, bool isConstIterations, string unit) {
    int32_t iteration_cnt;
    if (isConstIterations) {
        iteration_cnt = 32 * 1024 * 1024;
    } else {
        iteration_cnt = (size / stride) * factor;
    }
    assert(size % stride == 0);
    assert(stride >= sizeof(size_t));

    auto pair = fabric.createArray(size, stride);
    char * array = pair.first;
    char **p = pair.second;

    long long avg = 0;
    for (int j = 0; j < iterCount; ++j) {
        auto start = chrono::high_resolution_clock::now();
        for (int i = 0; i < iteration_cnt; ++i) {
            p = (char **) (*p);
        }
        auto end = chrono::high_resolution_clock::now();
        auto time = (end - start);
        if (unit == "ms") {
            avg += time / 1ms;
        } else if (unit == "us") {
            avg += time / 1us;
        } else if (unit == "ns") {
            avg += time / 1ns;
        } else {
            throw ("Unit " + unit + " not supported");
        }
    }

    delete [] array;
    return avg / iterCount;
}