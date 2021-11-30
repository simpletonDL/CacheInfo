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
    size_t space = 32 * 32 * 1024 * 1024;
    void *array_old = new char[space];
    char *array = (char *) align(1 << 20, size, array_old, space);
    assert(array != nullptr);

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



// stride, iterCount, factor
long long loadsFixed(int32_t stride, int32_t iterCount, size_t factor,
                     ArrayFabric &fabric, const string& unit) {
    assert(stride >= sizeof(size_t));

    int32_t size = stride * iterCount;


    auto pair = fabric.createArray(size, stride);
    char *array = pair.first;
    char **p_start = pair.second;

    long long avg = 0;

    // Запоминаем в кеше элементы
    char **p = p_start;
    for (int i = 0; i < iterCount; ++i) {
        p = (char **) (*p);
    }

    auto start = chrono::high_resolution_clock::now();
    auto end = start;
    switch (iterCount) {
        case 1: {
            start = chrono::high_resolution_clock::now();
            for (int j = 0; j < factor; ++j) {
                for (int i = 0; i < 1; ++i) {
                    p = (char **) (*p);
                }
            }
            end = chrono::high_resolution_clock::now();
            break;
        }
        case 2: {
            start = chrono::high_resolution_clock::now();
            for (int j = 0; j < factor; ++j) {
                for (int i = 0; i < 2; ++i) {
                    p = (char **) (*p);
                }
            }
            end = chrono::high_resolution_clock::now();
            break;
        }
        case 3: {
            start = chrono::high_resolution_clock::now();
            for (int j = 0; j < factor; ++j) {
                for (int i = 0; i < 3; ++i) {
                    p = (char **) (*p);
                }
            }
            end = chrono::high_resolution_clock::now();
            break;
        }
        case 4: {
            start = chrono::high_resolution_clock::now();
            for (int j = 0; j < factor; ++j) {
                for (int i = 0; i < 4; ++i) {
                    p = (char **) (*p);
                }
            }
            end = chrono::high_resolution_clock::now();
            break;
        }
        case 5: {
            start = chrono::high_resolution_clock::now();
            for (int j = 0; j < factor; ++j) {
                for (int i = 0; i < 5; ++i) {
                    p = (char **) (*p);
                }
            }
            end = chrono::high_resolution_clock::now();
            break;
        }
        case 6: {
            start = chrono::high_resolution_clock::now();
            for (int j = 0; j < factor; ++j) {
                for (int i = 0; i < 6; ++i) {
                    p = (char **) (*p);
                }
            }
            end = chrono::high_resolution_clock::now();
            break;
        }
        case 7: {
            start = chrono::high_resolution_clock::now();
            for (int j = 0; j < factor; ++j) {
                for (int i = 0; i < 7; ++i) {
                    p = (char **) (*p);
                }
            }
            end = chrono::high_resolution_clock::now();
            break;
        }
        case 8: {
            start = chrono::high_resolution_clock::now();
            for (int j = 0; j < factor; ++j) {
                for (int i = 0; i < 8; ++i) {
                    p = (char **) (*p);
                }
            }
            end = chrono::high_resolution_clock::now();
            break;
        }
        case 9: {
            start = chrono::high_resolution_clock::now();
            for (int j = 0; j < factor; ++j) {
                for (int i = 0; i < 9; ++i) {
                    p = (char **) (*p);
                }
            }
            end = chrono::high_resolution_clock::now();
            break;
        }
        case 10: {
            start = chrono::high_resolution_clock::now();
            for (int j = 0; j < factor; ++j) {
                for (int i = 0; i < 10; ++i) {
                    p = (char **) (*p);
                }
            }
            end = chrono::high_resolution_clock::now();
            break;
        }
        case 11: {
            start = chrono::high_resolution_clock::now();
            for (int j = 0; j < factor; ++j) {
                for (int i = 0; i < 11; ++i) {
                    p = (char **) (*p);
                }
            }
            end = chrono::high_resolution_clock::now();
            break;
        }
        case 12: {
            start = chrono::high_resolution_clock::now();
            for (int j = 0; j < factor; ++j) {
                for (int i = 0; i < 12; ++i) {
                    p = (char **) (*p);
                }
            }
            end = chrono::high_resolution_clock::now();
            break;
        }
        case 13: {
            start = chrono::high_resolution_clock::now();
            for (int j = 0; j < factor; ++j) {
                for (int i = 0; i < 13; ++i) {
                    p = (char **) (*p);
                }
            }
            end = chrono::high_resolution_clock::now();
            break;
        }
        case 14: {
            start = chrono::high_resolution_clock::now();
            for (int j = 0; j < factor; ++j) {
                for (int i = 0; i < 14; ++i) {
                    p = (char **) (*p);
                }
            }
            end = chrono::high_resolution_clock::now();
            break;
        }
        case 15: {
            start = chrono::high_resolution_clock::now();
            for (int j = 0; j < factor; ++j) {
                for (int i = 0; i < 15; ++i) {
                    p = (char **) (*p);
                }
            }
            end = chrono::high_resolution_clock::now();
            break;
        }
        case 16: {
            start = chrono::high_resolution_clock::now();
            for (int j = 0; j < factor; ++j) {
                for (int i = 0; i < 16; ++i) {
                    p = (char **) (*p);
                }
            }
            end = chrono::high_resolution_clock::now();
            break;
        }
        case 17: {
            start = chrono::high_resolution_clock::now();
            for (int j = 0; j < factor; ++j) {
                for (int i = 0; i < 17; ++i) {
                    p = (char **) (*p);
                }
            }
            end = chrono::high_resolution_clock::now();
            break;
        }
        case 18: {
            start = chrono::high_resolution_clock::now();
            for (int j = 0; j < factor; ++j) {
                for (int i = 0; i < 18; ++i) {
                    p = (char **) (*p);
                }
            }
            end = chrono::high_resolution_clock::now();
            break;
        }
        case 19: {
            start = chrono::high_resolution_clock::now();
            for (int j = 0; j < factor; ++j) {
                for (int i = 0; i < 19; ++i) {
                    p = (char **) (*p);
                }
            }
            end = chrono::high_resolution_clock::now();
            break;
        }
        case 20: {
            start = chrono::high_resolution_clock::now();
            for (int j = 0; j < factor; ++j) {
                for (int i = 0; i < 20; ++i) {
                    p = (char **) (*p);
                }
            }
            end = chrono::high_resolution_clock::now();
            break;
        }
        case 21: {
            start = chrono::high_resolution_clock::now();
            for (int j = 0; j < factor; ++j) {
                for (int i = 0; i < 21; ++i) {
                    p = (char **) (*p);
                }
            }
            end = chrono::high_resolution_clock::now();
            break;
        }
        case 22: {
            start = chrono::high_resolution_clock::now();
            for (int j = 0; j < factor; ++j) {
                for (int i = 0; i < 22; ++i) {
                    p = (char **) (*p);
                }
            }
            end = chrono::high_resolution_clock::now();
            break;
        }
        case 23: {
            start = chrono::high_resolution_clock::now();
            for (int j = 0; j < factor; ++j) {
                for (int i = 0; i < 23; ++i) {
                    p = (char **) (*p);
                }
            }
            end = chrono::high_resolution_clock::now();
            break;
        }
        case 24: {
            start = chrono::high_resolution_clock::now();
            for (int j = 0; j < factor; ++j) {
                for (int i = 0; i < 24; ++i) {
                    p = (char **) (*p);
                }
            }
            end = chrono::high_resolution_clock::now();
            break;
        }
        case 25: {
            start = chrono::high_resolution_clock::now();
            for (int j = 0; j < factor; ++j) {
                for (int i = 0; i < 25; ++i) {
                    p = (char **) (*p);
                }
            }
            end = chrono::high_resolution_clock::now();
            break;
        }
        case 26: {
            start = chrono::high_resolution_clock::now();
            for (int j = 0; j < factor; ++j) {
                for (int i = 0; i < 26; ++i) {
                    p = (char **) (*p);
                }
            }
            end = chrono::high_resolution_clock::now();
            break;
        }
        case 27: {
            start = chrono::high_resolution_clock::now();
            for (int j = 0; j < factor; ++j) {
                for (int i = 0; i < 27; ++i) {
                    p = (char **) (*p);
                }
            }
            end = chrono::high_resolution_clock::now();
            break;
        }
        case 28: {
            start = chrono::high_resolution_clock::now();
            for (int j = 0; j < factor; ++j) {
                for (int i = 0; i < 28; ++i) {
                    p = (char **) (*p);
                }
            }
            end = chrono::high_resolution_clock::now();
            break;
        }
        case 29: {
            start = chrono::high_resolution_clock::now();
            for (int j = 0; j < factor; ++j) {
                for (int i = 0; i < 29; ++i) {
                    p = (char **) (*p);
                }
            }
            end = chrono::high_resolution_clock::now();
            break;
        }
        case 30: {
            start = chrono::high_resolution_clock::now();
            for (int j = 0; j < factor; ++j) {
                for (int i = 0; i < 30; ++i) {
                    p = (char **) (*p);
                }
            }
            end = chrono::high_resolution_clock::now();
            break;
        }
        case 31: {
            start = chrono::high_resolution_clock::now();
            for (int j = 0; j < factor; ++j) {
                for (int i = 0; i < 31; ++i) {
                    p = (char **) (*p);
                }
            }
            end = chrono::high_resolution_clock::now();
            break;
        }
        case 32: {
            start = chrono::high_resolution_clock::now();
            for (int j = 0; j < factor; ++j) {
                for (int i = 0; i < 32; ++i) {
                    p = (char **) (*p);
                }
            }
            end = chrono::high_resolution_clock::now();
            break;
        }
        case 33: {
            start = chrono::high_resolution_clock::now();
            for (int j = 0; j < factor; ++j) {
                for (int i = 0; i < 33; ++i) {
                    p = (char **) (*p);
                }
            }
            end = chrono::high_resolution_clock::now();
            break;
        }
        case 34: {
            start = chrono::high_resolution_clock::now();
            for (int j = 0; j < factor; ++j) {
                for (int i = 0; i < 34; ++i) {
                    p = (char **) (*p);
                }
            }
            end = chrono::high_resolution_clock::now();
            break;
        }
    }

    auto time = (end - start);
    if (unit == "ms") {
        avg = time / 1ms;
    } else if (unit == "us") {
        avg = time / 1us;
    } else if (unit == "ns") {
        avg = time / 1ns;
    } else {
        throw ("Unit " + unit + " not supported");
    }

    delete [] array;
    return avg / factor;
}