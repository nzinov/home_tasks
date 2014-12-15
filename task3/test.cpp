#include "heap_array.h"
#include "binomial_heap.h"
#include "swapping_heap.h"
#include "leftist_heap.h"
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

template <typename heap_t> void fine_print_test(const char* name, const vector<TestCall>& test) {
    std::cout << "Test of " << name << " Heap, run in " << run_test<SwappingHeap<SwappingBinaryNode>>(test) << "s" << std::endl;
}

int main() {
    vector<TestCall> test(TEST_LENGTH);
    std::default_random_engine generator;
    std::discrete_distribution<char> what_function {2, 9, 4, 1};
    std::uniform_int_distribution<key_type> what_arg;
    for (size_t i = 0; i < TEST_LENGTH; ++i)  {
        test[i] = TestCall{static_cast<FuncName>(what_function(generator)), what_arg(generator), what_arg(generator)};
    }
    fine_print_test<BinomialHeap>("Binomial", test);
    fine_print_test<SwappingHeap<SwappingBinaryNode>>("Skew", test);
    fine_print_test<LeftistHeap>("Leftist", test);
}

