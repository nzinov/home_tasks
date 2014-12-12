#include "heap.h"
#include <vector>

using std::vector;
using std::size_t;

struct Node {
    key_type key;
    Node* brother;
    Node* child;

    Node(key_type key) : key(key) {
        brother = nullptr;
        child = nullptr;
    }

    Node* merge(Node* other) {
        Node* head = this->key < other->key ? this : other;
        Node* tail = this->key < other->key ? other : this;
        if (head->child == nullptr) {
            head->child = tail;
        }
        else {
            Node* child = head->child;
            while (child->brother != nullptr) {
                child = child->brother;
            }
            child->brother = tail;
        }
        return head;
    }
};

class BinomialHeap : public Heap {
    vector<Node*> trees;
    Node* tree(size_t);

    public:
    virtual key_type get_min() const;
    virtual key_type extract_min();
    virtual void insert(key_type);
    virtual void merge(Heap*);
    virtual ~BinomialHeap() {}
};

Node* BinomialHeap::tree(size_t i) {
    if (i >= trees.size()) {
        return nullptr;
    }
    else {
        return trees[i];
    }
}

key_type BinomialHeap::get_min() const {
    if (trees.size() == 0) {
        throw BinomialHeap();
    }
    key_type m = -1;
    for (Node* el : trees) {
        m = std::min(el->key, m);
    }
    return m;
}

void BinomialHeap::merge(Heap* o) {
    BinomialHeap* other = dynamic_cast<BinomialHeap*>(o);
    if (other == nullptr) {
        throw BinomialHeap();
    }
    size_t size = std::max(this->trees.size(), other->trees.size());
    this->trees.resize(size);
    Node* carry = nullptr;
    for (size_t i = 0; i < size; ++i) {
        size_t count = 3;
        Node* operands[3] = {this->tree(i), other->tree(i), carry};
        for (size_t cur = 0; cur < count;) {
            if (operands[cur] == nullptr) {
                operands[cur] = operands[count-1];
                --count;
            }
            else {
                ++cur;
            }
        }
        if (count == 1) {
            this->trees[i] = operands[0];
        }
        else if (count >= 2) {
            carry = operands[0]->merge(operands[1]);
            this->trees[i] = operands[2];
        }
    }
    if (carry != nullptr) {
        this->trees.push_back(carry);
    }
    delete other;
}

key_type BinomialHeap::extract_min() {
    if (trees.size() == 0) {
        throw BinomialHeap();
    }
    size_t min_node = 0;
    for (size_t i = 1; i < trees.size(); ++i) {
        if (trees[i] != nullptr && (trees[min_node] == nullptr || trees[i]->key < trees[min_node]->key)) {
            min_node = i;
        }
    }
    Node* node = trees[min_node];
    if (node == nullptr) {
        throw BinomialHeap();
    }
    key_type min = node->key;
    if (node->child != nullptr) {
        BinomialHeap* new_heap = new BinomialHeap();
        Node* child = node->child;
        while (child != nullptr) {
            new_heap->trees.push_back(child);
            child = child->brother;
        }
        trees[min_node] = nullptr;
        this->merge(new_heap);
    }
    else {
        trees[min_node] = nullptr;
    }
    delete node;
    return min;
}

void BinomialHeap::insert(key_type key) {
    Node* node = new Node(key);
    BinomialHeap* heap = new BinomialHeap();
    heap->trees.push_back(node);
    this->merge(heap);
}
