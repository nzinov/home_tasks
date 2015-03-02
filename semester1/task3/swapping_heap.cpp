#include "swapping_heap.h"
#include <algorithm>

SwappingBinaryNode* SwappingBinaryNode::merge(SwappingBinaryNode* a, SwappingBinaryNode* b) {
    if (a == nullptr) {
        return b;
    }
    if (b == nullptr) {
        return a;
    }
    if (a->key < b->key) {
        std::swap(a, b);
    }
    a->right = SwappingBinaryNode::merge(a->right, b);
    if (a->should_swap()) {
        std::swap(a->left, a->right);
    }
    a->maintain();
    return a;
}

void SwappingBinaryNode::maintain() {
}

bool SwappingBinaryNode::should_swap() {
    return true;
}
