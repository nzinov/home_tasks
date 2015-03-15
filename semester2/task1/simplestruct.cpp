#include "simplestruct.h"
#include <algorithm>

void SimpleStruct::insert(int value, size_t pos) {
    data.insert(data.begin() + pos, value);
}

void SimpleStruct::set(int value, size_t pos) {
    data[pos] = value;
}

long long SimpleStruct::sum(size_t left, size_t right) {
    return std::accumulate(data.begin() + left, data.begin() + right, 0);
}

void SimpleStruct::next_permutation(size_t left, size_t right) {
    std::next_permutation(data.begin() + left, data.begin() + right);
}
