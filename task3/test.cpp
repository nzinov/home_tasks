#include "heap_array.cpp"
#include "binomial_heap.cpp"
#include <random>
#include <ctime>
#include <vector>
#include <iostream>

using std::vector;

const size_t TEST_LENGTH = 1000000;

template <typename heap_t> double run_test(const vector<TestCall>& test) {
    clock_t start = clock();
    HeapArray<heap_t> array;
    for (TestCall el: test) {
        array.call(el);
    }
    return static_cast<double>(clock() - start) / CLOCKS_PER_SEC;
}

int main() {
    vector<TestCall> test(TEST_LENGTH);
    std::default_random_engine generator;
    std::discrete_distribution<char> what_function {2, 9, 4, 1};
    std::uniform_int_distribution<key_type> what_arg;
    for (size_t i = 0; i < TEST_LENGTH; ++i)  {
        test[i] = TestCall{static_cast<FuncName>(what_function(generator)), what_arg(generator), what_arg(generator)};
    }
    std::cout << "run in " <<run_test<BinomialHeap>(test) << "s";
}

