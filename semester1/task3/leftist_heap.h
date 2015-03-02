#ifndef leftist_heap
#define leftist_heap
#include "swapping_heap.h"
#include <algorithm>

using std::size_t;

struct LeftistNode : SwappingBinaryNode {
    size_t dist;
    LeftistNode(key_type key) : SwappingBinaryNode(key), dist(1) {};
    virtual void maintain();
    virtual bool should_swap();
    virtual ~LeftistNode() {}
};

typedef SwappingHeap<LeftistNode> LeftistHeap;
#endif
