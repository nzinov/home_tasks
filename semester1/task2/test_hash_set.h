#include "hash_set.h"
#include <set>

class TestHashSet {
    vector<ui32> keys;
    std::set<ui32> table;
    public:
    virtual void init(const vector<ui32>& keys);
    virtual bool has(ui32 key);
    virtual bool is_possible_key(ui32 key);
    virtual void insert(ui32 key, bool value = true);
    virtual void erase(ui32 key);
    virtual size_t size() const;
};
