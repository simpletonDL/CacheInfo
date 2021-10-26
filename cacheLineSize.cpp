#include <iostream>
#include <map>
#include <random>

using namespace std;

struct ArrayFabric {
    virtual pair<char*, char**> createArray(int32_t size, int32_t stride) = 0;
};

struct PreviousStep: ArrayFabric {
    pair<char*, char**> createArray(int32_t size, int32_t stride) override {
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
};

void myShuffle(vector<int32_t> &v) {
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(v.begin(), v.end(), g);
}

struct RandomStep: ArrayFabric {
    pair<char*, char**> createArray(int32_t size, int32_t stride) override {
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
};

long long loads(int32_t size, int32_t stride, int32_t factor, int32_t iterCount, ArrayFabric &fabric, bool isConstIterations) {
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
        avg += (end - start) / 1us;
    }

    delete [] array;
    return avg / iterCount;
}

int main() {
    int32_t startStride = 1 << 3;
    int32_t endStride = 1 << 12;
    RandomStep fabric;

    int32_t arrSize = 32 * 1024 * 1024;

    map<int32_t, long long> results;
    for (int stride = startStride; stride <= endStride; stride <<= 1) {
        cout << "Computing stride " << stride << "\n";
        results[stride] = loads(arrSize, stride, 10, 5, fabric, false);
    }
    for (auto p: results) {
        cout << p.first << ": " << p.second << "us\n";
    }
    return 0;
}
