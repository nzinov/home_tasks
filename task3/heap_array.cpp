#include <vector>
#include "heap.h"

using std::vector;
using std::size_t;

enum FuncName {
    add_heap,
    insert,
    extract_min,
    merge
};

struct TestCall {
    FuncName function;
    key_type arg_a;
    key_type arg_b;
};

template <typename heap_t> class HeapArray {
    vector<Heap*> array;
    Heap*& get(size_t index) {
        return array[index % array.size()];
    }
public:
    void add_heap(key_type key) {
        Heap* heap = new heap_t();
        heap->insert(key);
        array.push_back(heap);
    }

    void insert(size_t index, key_type key) {
        if (array.size() == 0)
            return;
        get(index)->insert(key);
    }

    key_type extract_min(size_t index) {
        if (array.size() == 0)
            return 0;
        return get(index)->extract_min();
    }

    void merge(size_t a, size_t b) {
        if (array.size() == 0)
            return;
        get(a)->merge(get(b));
        get(b) = array.back();
        array.pop_back();
    }
    
    void call(TestCall test) {
        switch (test.function) {
            case FuncName::add_heap:
                add_heap(test.arg_a);
                break;
            case FuncName::insert:
                insert(test.arg_a, test.arg_b);
                break;
            case FuncName::extract_min:
                extract_min(test.arg_a);
                break;
            case FuncName::merge:
                merge(test.arg_a, test.arg_b);
                break;
        }
    }
};

