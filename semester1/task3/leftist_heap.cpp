#include "leftist_heap.h"

void LeftistNode::maintain() {
    dist = 0;
    LeftistNode* _left = dynamic_cast<LeftistNode*>(left);
    LeftistNode* _right = dynamic_cast<LeftistNode*>(right);
    if (left) {
        dist = std::max(dist, _left->dist);
    }
    if (right) {
        dist = std::max(dist, _right->dist);
    }
    ++dist;
}

bool LeftistNode::should_swap() {
    LeftistNode* _left = dynamic_cast<LeftistNode*>(left);
    LeftistNode* _right = dynamic_cast<LeftistNode*>(right);
    return (left ? _left->dist : 0) < (right ? _right->dist : 0);
}
