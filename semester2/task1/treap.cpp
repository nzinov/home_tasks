#include <algorithm>
#include <random>
#include <cassert>
#include <iostream>
#include "permutationstruct.h"

const int INF = 100;

struct Node {
    int min;
    int max;
    int value;
    int priority;
    int count;
    bool is_decreasing;
    bool is_increasing;
    long long sum;
    Node* left;
    Node* right;
    bool must_reverse;

    Node() : min(INF), max(-INF), count(0), is_decreasing(true), is_increasing(true), sum(0) {};

    Node(int value) : min(value), max(value), value(value), count(1), is_decreasing(true), is_increasing(true), sum(value) {
        priority = rand();
    }

    static Node null_obj;

    static Node* get(Node* pointer) {
        if (pointer == nullptr) {
            return &null_obj;
        }
        return pointer;
    }

    void update() {
        min = value;
        Node* l = get(left);
        Node* r = get(right);
        min = std::min({l->min, r->min, value});
        max = std::max({l->max, r->max, value});
        sum = value + l->sum + r->sum;
        count = 1 + l->count + r->count;
        is_increasing = l->is_increasing && l->max < value && r->is_increasing && r->min > value;
        is_decreasing = l->is_decreasing && l->min >= value && r->is_decreasing && r->max <= value;
    }
    void push() {
        if (must_reverse) {
            if (left != nullptr) {
                left->must_reverse = !left->must_reverse;
            }
            if (right != nullptr) {
                right->must_reverse = !right->must_reverse;
            }
            must_reverse = false;
            std::swap(left, right);
            std::swap(is_decreasing, is_increasing);
        }
    }
    void reverse() {
        must_reverse = !must_reverse;
    }

    static Node* merge(Node* a, Node* b) {
        if (a == nullptr) {
            return b;
        }
        if (b == nullptr) {
            return a;
        }
        if (a->priority > b->priority) {
            a->push();
            a->right = Node::merge(a->right, b);
            a->update();
            return a;
        } else {
            b->push();
            b->left = Node::merge(a, b->left);
            b->update();
            return b;
        }
    }

    // splits node into into left = [; key) and right = [key; ]
    static void split(size_t pos, Node* node, Node*& left, Node*& right, size_t shift = 0) {
        if (node == nullptr) {
            left = nullptr;
            right = nullptr;
            return;
        }
        Node* left_buf;
        Node* right_buf;
        node->push();
        size_t cur_pos = shift + get(node->left)->count;
        if (cur_pos >= pos) {
            Node::split(pos, node->left, left_buf, right_buf, shift);
            node->left = right_buf;
            node->update();
            left = left_buf;
            right = node;
        } else {
            Node::split(pos, node->right, left_buf, right_buf, cur_pos + 1);
            node->right = left_buf;
            node->update();
            left = node;
            right = right_buf;
        }
    };
    Node* get(size_t pos, size_t shift = 0) {
        push();
        size_t cur_pos = shift + Node::get(left)->count;
        if (pos == cur_pos) {
            return this;
        }
        if (pos < cur_pos) {
            return left->get(pos, shift);
        } else {
            return right->get(pos, cur_pos + 1);
        }
    }
    void set(size_t pos, int val, size_t shift = 0) {
        push();
        size_t cur_pos = shift + Node::get(left)->count;
        if (pos == cur_pos) {
            value = val;
        } else if (pos < cur_pos) {
            left->set(pos, val, shift);
        } else {
            right->set(pos ,val, cur_pos + 1);
        }
        update();
    }
    void print() {
        if (count == 0) {
            std::cout << "null";
            return;
        }
        push();
        std::cout << "(" << value << " ";
        get(left)->print();
        std::cout << ' ';
        get(right)->print();
        std::cout << ')';
    } 
};

Node Node::null_obj;

class Treap : PermutationStruct {
public:
    virtual void insert(int value, size_t pos);
    virtual void set(int value, size_t pos);
    virtual int get(size_t pos);
    virtual long long sum(size_t left, size_t right);
    virtual void next_permutation(size_t left, size_t right);
    Treap() : root(nullptr) {};
    Node* root;
private:
    //finds length of the greatest decreasing suffix with elements greater than 'max'
    size_t find_suffix(Node* cur, int max) {
        if (cur == nullptr) {
            return 0;
        }
        cur->push();
        if (cur->is_decreasing && cur->min >= max) {
            return cur->count;
        }
        while (cur->right != nullptr && !cur->right->is_decreasing) {
            cur = cur->right;
        }
        size_t length = 0;
        if (cur->right != nullptr) {
            if (cur->right->min < max) {
                return 0;
            }
            max = cur->right->max;
            length = cur->right->count;
        }
        if (cur->value >= max) {
            length += find_suffix(cur->left, cur->value) + 1;
        }
        return length;
    }
    //finds position of the last element that is greater than 'val'
    size_t find_greater(Node* cur, int val, size_t shift = 0) {
        cur->push();
        assert(cur->is_decreasing);
        if (Node::get(cur->right)->max > val) {
            return find_greater(cur->right, val, shift + Node::get(cur->left)->count + 1);
        } else {
            if (cur->value > val) {
                return shift + Node::get(cur->left)->count;
            }
            return find_greater(cur->left, val, shift);
        }
    }
};

void Treap::next_permutation(size_t left, size_t right) {
    Node *left_buf, *right_buf;
    Node::split(left, root, left_buf, root);
    Node::split(right, root, root, right_buf);
    size_t suffix_length = find_suffix(root, -INF);
    size_t a = root->count - suffix_length;
    if (a == 0) {
        root->reverse();
    } else {
        --a;
        Node* suffix;
        Node::split(a+1, root, root, suffix);
        int a_value = root->get(a)->value;
        size_t b = find_greater(suffix, a_value);
        root->set(a, suffix->get(b)->value);
        suffix->set(b, a_value);
        suffix->reverse();
        root = Node::merge(root, suffix);
    }
    root = Node::merge(left_buf, root);
    root = Node::merge(root, right_buf);
}
void Treap::insert(int value, size_t pos) {
    Node* buf;
    Node::split(pos, root, root, buf);
    Node* new_node = new Node(value);
    root = Node::merge(root, new_node);
    root = Node::merge(root, buf);
}
void Treap::set(int value, size_t pos) {
    root->set(pos, value);
}
int Treap::get(size_t pos) {
    return root->get(pos)->value;
}
long long Treap::sum(size_t left, size_t right) {
    Node *left_buf, *right_buf;
    Node::split(left, root, left_buf, root);
    Node::split(right, root, root, right_buf);
    long long ans = root->sum;
    root = Node::merge(left_buf, root);
    root = Node::merge(root, right_buf);
    return ans;
}

int main() {
    Treap treap;
    treap.insert(1, 0);
    treap.insert(2, 1);
    treap.insert(3, 2);
    treap.insert(4, 3);
    for (int i = 0; i < 30; ++i) {
        for (int pos = 0; pos < 4; ++pos) {
            std::cout << treap.get(pos) << ' ';
        }
        std::cout << std::endl;
        treap.next_permutation(0, 5);
        treap.root->print();
    }
}
