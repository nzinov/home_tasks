#include <vector>
#include <random>

using std::vector;
using std::size_t;

typedef unsigned int ui32;
typedef unsigned long long ui64;

struct KeyCollision {
};

struct DuplicateKeys {
};

size_t get_prime_above(size_t m) {
    for (; true; ++m) {
        bool prime = true;
        for (size_t div = 2; div*div < m; ++div) {
            if (m % div == 0) {
                prime  = false;
                break;
            }
        }
        if (prime) {
            return m;
        }
    }
}

struct UniversalHashFunction {
    size_t a, b, m;
    const ui64 P = (1 << 32) + 15;

    void init(size_t m) {
        this->m = m;
    }

    void randomize() {
        a = rand() % p + 1;
        b = rand() % p;
    }

    size_t operator() (ui32 x) const {
        return (a * x + b) % p % m;
    }
};

struct SecondLevelTable {
    UniversalHashFunction hash;
    vector<ui32> keys;
    vector<bool> table;
    vector<ui32> map;

    bool get_iter(ui32 key, vector<bool>::iterator& iter) {
        if (keys.empty()) {
            return false;
        }
        size_t index = hash(key);
        if (map[index] != key) {
            return false;
        }
        iter =  table.begin() + index;
        return true;
    }

    void add(ui32 key) {
        keys.push_back(key);
    }

    void init() {
        size_t n = keys.size();
        if (!n) {
            return;
        }
        size_t m = n*n;
        hash.init(m);
        table.resize(m);
        map.resize(m);
        bool fit = false;
        while (!fit) {
            hash.randomize();
            fit = true;
            for (size_t i = 0; i < n; i++) {
                size_t index = hash(keys[i]);
                if (table[index]) {
                    if (map[index] == keys[i]) {
                        throw DuplicateKeys();
                    }
                    fit = false;
                    break;
                }
                table[index] = true;
                map[index] = keys[i];
            }
            std::fill(table.begin(), table.end(), false);
        }
    }

    size_t size() const {
        return table.size();
    }
};

class PerfectHashSet {
    UniversalHashFunction hash;
    vector<ui32> keys;
    vector<SecondLevelTable> table;

    bool get_iter(ui32 key, vector<bool>::iterator& iter ) {
        return table[hash(key)].get_iter(key, iter);
    }

    public:
    void init(const vector<ui32>& keys) {
        this->keys = keys;
        size_t m = keys.size();
        table.resize(m);
        hash.init(m);
        while (true) {
            hash.randomize();
            for (ui32 key : keys) {
                table[hash(key)].add(key);
            }
            size_t total_size = 0;
            for (size_t i = 0; i < m; ++i) {
                table[i].init();
                total_size += table[i].size();
            }
            if (total_size <= 4*m) {
                break;
            }
        }
    }

    bool has(ui32 key) {
        vector<bool>::iterator iter;
        return get_iter(key, iter) && *iter;
    }

    void insert(ui32 key, bool value = true) {
        vector<bool>::iterator iter;
        if (!get_iter(key, iter)) {
            throw KeyCollision();
        }
        *iter = value;
    }

    void erase(ui32 key) {
        insert(key, false);
    }
};
