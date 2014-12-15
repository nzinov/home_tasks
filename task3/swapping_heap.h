#ifndef SWAPPING_HEAP
#define SWAPPING_HEAP
#include "heap.h"

struct SwappingBinaryNode {
    key_type key;
    SwappingBinaryNode* left;
    SwappingBinaryNode* right;

    SwappingBinaryNode(key_type key) : key(key), left(nullptr), right(nullptr) {}

    static SwappingBinaryNode* merge(SwappingBinaryNode* a, SwappingBinaryNode* b);
    virtual void maintain();
    virtual bool should_swap();
    virtual ~SwappingBinaryNode() {}
};

template <typename node_t = SwappingBinaryNode> class SwappingHeap : public Heap {
    SwappingBinaryNode* root;
    public:
    SwappingHeap() : root(nullptr) {}
    virtual key_type get_min() const;
    virtual key_type extract_min();
    virtual void insert(key_type);
    virtual void merge(Heap*);
    virtual bool is_empty() const;
    virtual ~SwappingHeap() {}
};

template <typename node_t> key_type SwappingHeap<node_t>::get_min()  const {
    if (root) {
        return root->key;
    }
    return -1;
}

template <typename node_t> key_type SwappingHeap<node_t>::extract_min() {
    if (!root) {
        throw EmptyHeapException();
    }
    key_type min = root->key;
    SwappingBinaryNode* old_root = root;
    root = SwappingBinaryNode::merge(root->left, root->right);
    delete old_root;
    return min;
}

template <typename node_t> void SwappingHeap<node_t>::insert(key_type key) {
    SwappingHeap* new_heap = new SwappingHeap();
    SwappingBinaryNode* new_root = new SwappingBinaryNode(key);
    new_heap->root = new_root;
    merge(new_heap);
}

template <typename node_t> void SwappingHeap<node_t>::merge(Heap* unsafe_other) {
    SwappingHeap* other = dynamic_cast<SwappingHeap*>(unsafe_other);
    if (!other) {
        throw InconsistentTypeException();
    }
    root = node_t::merge(root, other->root);
    delete other;
}

template <typename node_t> bool SwappingHeap<node_t>::is_empty() const {
    return !root;
}
#endif

