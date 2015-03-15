#include <climits>
#include <cstddef>
#include <random>
#include "permutationstruct.h"

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
    static Node null_obj;

    Node() : min(INT_MAX), max(INT_MIN), count(0), is_decreasing(true), is_increasing(true), sum(0), left(nullptr), right(nullptr), must_reverse(false) {};

    Node(int value) : min(value), max(value), value(value), count(1),
            is_decreasing(true), is_increasing(true), sum(value), left(nullptr), right(nullptr), must_reverse(false) {
        priority = rand();
    }

    static Node* safe(Node* pointer);
    void update();
    void push();
    void reverse();

    //returns pointer to a + b
    static Node* merge(Node* a, Node* b);

    // splits node into into left = [0; pos) and right = [pos; end]
    static void split(size_t pos, Node* node, Node*& left, Node*& right, size_t shift = 0);;
    Node* get(size_t pos, size_t shift = 0);
    void set(size_t pos, int val, size_t shift = 0);
    void print();
};

class Treap : public PermutationStruct {
public:
    virtual void insert(int value, size_t pos);
    virtual void set(int value, size_t pos);
    virtual int get(size_t pos);
    virtual long long sum(size_t left, size_t right);
    virtual void next_permutation(size_t left, size_t right);
    Treap() : root(nullptr) {};
    Node* root;
private:
    //finds length of the greatest decreasing suffix of cur's subtree with elements greater than 'max'
    size_t find_suffix(Node* cur, int max);
    //finds position of the last element that is greater than 'val' in cur's subtree
    size_t find_greater(Node* cur, int val, size_t shift = 0);
};
