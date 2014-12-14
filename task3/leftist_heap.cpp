#include <algorithm>
#include "heap.h"

struct SwappingBinaryNode {
    key_type key;
    SwappingBinaryNode* left;
    SwappingBinaryNode* right;

    SwappingBinaryNode(key_type key) : key(key), left(nullptr), right(nullptr) {}

    static SwappingBinaryNode* merge(SwappingBinaryNode* a, SwappingBinaryNode* b) {
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

    void maintain() {
    }

    bool should_swap() {
        return true;
    }
};

template <typename node_t> class MergeableBinaryHeap : Heap {
    node_t* root;
public:
    MergeableBinaryHeap() : root(nullptr) {}
    virtual key_type get_min() const;
    virtual key_type extract_min();
    virtual void insert(key_type);
    virtual void merge(Heap*);
    virtual ~MergeableBinaryHeap() {}
};

template <typename node_t> key_type MergeableBinaryHeap<node_t>::get_min()  const {
    if (root) {
        return root->key;
    }
    return -1;
}

template <typename node_t> key_type MergeableBinaryHeap<node_t>::extract_min() {
    if (!root) {
        throw MergeableBinaryHeap();
    }
    key_type min = root->key;
    SwappingBinaryNode* old_root = root;
    root = SwappingBinaryNode::merge(root->left, root->right);
    delete old_root;
    return min;
}

template <typename node_t> void MergeableBinaryHeap<node_t>::insert(key_type key) {
    MergeableBinaryHeap* new_heap = new MergeableBinaryHeap();
    SwappingBinaryNode* new_root = new SwappingBinaryNode(key);
    new_heap->root = new_root;
    merge(new_heap);
}

template <typename node_t> void MergeableBinaryHeap<node_t>::merge(Heap* unsafe_other) {
    MergeableBinaryHeap* other = dynamic_cast<MergeableBinaryHeap>(unsafe_other);
    if (!other) {
        throw MergeableBinaryHeap();
    }
    root = node_t::merge(root, other->root);
    delete other;
}

