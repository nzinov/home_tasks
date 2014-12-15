#ifndef BINOMIAL_HEAP
#define BINOMIAL_HEAP
#include <vector>
#include "heap.h"

using std::vector;
using std::size_t;

struct Node {
    key_type key;
    Node* brother;
    Node* child;

    Node(key_type key) : key(key), brother(nullptr), child(nullptr) {}
    Node* merge(Node*);
};

class BinomialHeap : public Heap {
    vector<Node*> trees;
    Node* tree(size_t);
    public:
    virtual key_type get_min() const;
    virtual key_type extract_min();
    virtual void insert(key_type);
    virtual void merge(Heap*);
    virtual bool is_empty() const;
    virtual ~BinomialHeap() {};
};
#endif
