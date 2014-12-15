#include "binomial_heap.h"

Node* Node::merge(Node* other) {
    Node* head = this->key < other->key ? this : other;
    Node* tail = this->key < other->key ? other : this;
    if (!head->child) {
        head->child = tail;
    } else {
        Node* child = head->child;
        while (child->brother) {
            child = child->brother;
        }
        child->brother = tail;
    }
    return head;
}

Node* BinomialHeap::tree(size_t i) {
    if (i >= trees.size()) {
        return nullptr;
    } else {
        return trees[i];
    }
}

bool BinomialHeap::is_empty() const {
    if (trees.size() == 0) {
        return false;
    }
    for (Node* el : trees) {
        if (el) {
            return true;
        }
    }
    return false;
}

key_type BinomialHeap::get_min() const {
    if (is_empty()) {
        throw EmptyHeapException();
    }
    key_type m = -1;
    for (Node* el : trees) {
        m = std::min(el->key, m);
    }
    return m;
}

void BinomialHeap::merge(Heap* o) {
    BinomialHeap* other = dynamic_cast<BinomialHeap*>(o);
    if (!other) {
        throw InconsistentTypeException();
    }
    size_t size = std::max(this->trees.size(), other->trees.size());
    this->trees.resize(size);
    Node* carry = nullptr;
    for (size_t i = 0; i < size; ++i) {
        size_t count = 3;
        Node* operands[3] = {this->tree(i), other->tree(i), carry};
        carry = nullptr;
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
            this->trees[i] = (count == 3 ? operands[2] : nullptr);
        }
    }
    if (carry != nullptr) {
        this->trees.push_back(carry);
    }
    delete other;
}

key_type BinomialHeap::extract_min() {
    if (is_empty()) {
        throw EmptyHeapException();
    }
    size_t min_node = 0;
    for (size_t i = 1; i < trees.size(); ++i) {
        if (trees[i] != nullptr && (trees[min_node] == nullptr || trees[i]->key < trees[min_node]->key)) {
            min_node = i;
        }
    }
    Node* node = trees[min_node];
    key_type min = node->key;
    if (node->child) {
        BinomialHeap* new_heap = new BinomialHeap();
        Node* child = node->child;
        while (child) {
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
