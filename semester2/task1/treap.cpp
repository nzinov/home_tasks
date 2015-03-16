#include <iostream>
#include <algorithm>
#include <random>
#include <cassert>
#include <climits>
#include "permutationstruct.h"
#include "treap.h"

Node Node::null_obj;

Node* Node::safe(Node* pointer) {
    if (pointer == nullptr) {
        return &null_obj;
    }
    return pointer;
}

void Node::update() {
    min = value;
    Node* l = safe(left);
    Node* r = safe(right);
    min = std::min({l->min, r->min, value});
    max = std::max({l->max, r->max, value});
    sum = value + l->sum + r->sum;
    count = 1 + l->count + r->count;
    is_increasing = l->increasing() && l->max <= value && r->increasing() && r->min >= value;
    is_decreasing = l->decreasing() && l->min >= value && r->decreasing() && r->max <= value;
}

bool Node::decreasing() {
    return (must_reverse ? is_increasing : is_decreasing);
}

bool Node::increasing() {
    return (must_reverse ? is_decreasing : is_increasing);
}

void Node::push() {
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

void Node::reverse() {
    must_reverse = !must_reverse;
}

Node* Node::merge(Node* a, Node* b) {
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

void Node::print() {
    if (count == 0) {
        std::cout << "null";
        return;
    }
    push();
    std::cout << "(" << value << (decreasing() ? ">" : "") << " ";
    safe(left)->print();
    std::cout << ' ';
    safe(right)->print();
    std::cout << ')';
    std::cout.flush();
} 

void Node::split(size_t pos, Node* node, Node*& left, Node*& right, size_t shift) {
    if (node == nullptr) {
        left = nullptr;
        right = nullptr;
        return;
    }
    Node* left_buf;
    Node* right_buf;
    node->push();
    size_t cur_pos = shift + safe(node->left)->count;
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

Node* Node::get(size_t pos, size_t shift) {
    push();
    size_t cur_pos = shift + Node::safe(left)->count;
    if (pos == cur_pos) {
        return this;
    }
    if (pos < cur_pos) {
        return left->get(pos, shift);
    } else {
        return right->get(pos, cur_pos + 1);
    }
}

void Node::set(size_t pos, int val, size_t shift) {
    push();
    size_t cur_pos = shift + Node::safe(left)->count;
    if (pos == cur_pos) {
        value = val;
    } else if (pos < cur_pos) {
        left->set(pos, val, shift);
    } else {
        right->set(pos ,val, cur_pos + 1);
    }
    update();
}

size_t Treap::find_suffix(Node* cur, int max) {
    if (cur == nullptr) {
        return 0;
    }
    cur->push();
    if (cur->decreasing() && cur->min >= max) {
        return cur->count;
    }
    while (cur->right != nullptr && !cur->right->decreasing()) {
        cur = cur->right;
        cur->push();
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

size_t Treap::find_greater(Node* cur, int val, size_t shift) {
    cur->push();
    assert(cur->decreasing());
    assert(cur->max > val);
    if (Node::safe(cur->right)->max > val) {
        return find_greater(cur->right, val, shift + Node::safe(cur->left)->count + 1);
    } else {
        if (cur->value > val) {
            return shift + Node::safe(cur->left)->count;
        }
        return find_greater(cur->left, val, shift);
    }
}

void Treap::next_permutation(size_t left, size_t right) {
    Node *left_buf, *right_buf;
    Node::split(left, root, left_buf, root);
    Node::split(right - left, root, root, right_buf);
    size_t suffix_length = find_suffix(root, INT_MIN);
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
    Node::split(right - left, root, root, right_buf);
    long long ans = root->sum;
    root = Node::merge(left_buf, root);
    root = Node::merge(root, right_buf);
    return ans;
}
