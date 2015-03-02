#include <vector>
#include <random>
#include "hash_set.h"

using std::vector;
using std::size_t;

typedef unsigned int ui32;
typedef unsigned long long int ui64;

const ui64 mod32 = (1LLU << 32);
const ui64 P = mod32 + 15;

struct UniversalHashFunction {
    size_t m;
    ui64 a, b;

    void init(size_t m);
    void randomize();
    size_t operator() (ui32 x) const;
};

struct SecondLevelTable {
    UniversalHashFunction hash;
    vector<ui32> keys;
    vector<bool> table;
    vector<ui32> map;

    bool try_get_iter(ui32 key, vector<bool>::iterator& iter);
    bool check_duplicate_keys();
    void add(ui32 key);
    void clear();
    void init();
    size_t size() const;
};

class PerfectHashSet : HashSet {
    UniversalHashFunction hash;
    vector<ui32> keys;
    vector<SecondLevelTable> table;

    bool try_get_iter(ui32 key, vector<bool>::iterator& iter );

    public:
    virtual void init(const vector<ui32>& keys);
    virtual bool has(ui32 key);
    virtual bool is_possible_key(ui32 key);
    virtual void insert(ui32 key, bool value = true);
    virtual void erase(ui32 key);
    virtual size_t size() const;
};
