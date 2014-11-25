#ifndef HASH_SET
#define HASH_SET
#include <vector>

using std::vector;
using std::size_t;

typedef unsigned int ui32;

struct DuplicateKeys {
};

struct KeyCollision {
};

class HashSet {
    public:
    virtual void init(const vector<ui32>& keys) = 0;
    virtual bool has(ui32 key) = 0;
    virtual bool is_possible_key(ui32 key) = 0;
    virtual void insert(ui32 key, bool value = true) = 0;
    virtual void erase(ui32 key) = 0;
    virtual size_t size() const = 0;
};
#endif
