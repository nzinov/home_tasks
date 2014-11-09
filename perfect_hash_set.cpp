#include <vector>
#include <cstdlib>

using std::vector;

typedef long long int i64;
typedef unsigned int size_t;

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
    size_t a, b, m, p;

    void init(size_t m, size_t p) {
        this->m = m;
        this->p = p;
    }

    void randomize() {
        a = rand();
        b = rand();
    }

    size_t operator() (i64 x) const {
        return (a * (unsigned long long)x + b) % p % m;
    }
};

struct SecondLevelTable {
    UniversalHashFunction hash;
    vector<i64> keys;
    vector<bool> table;
    vector<i64> map;

    bool get_iter(i64 key, vector<bool>::iterator& iter) {
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

    void add(i64 key) {
        keys.push_back(key);
    }

    void init() {
        size_t n = keys.size();
        if (!n) {
            return;
        }
        size_t m = n*n;
        size_t p = get_prime_above(m);
        hash.init(m, p);
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
            for (size_t i = 0; i < m; i++) {
                table[i] = false;
            }
        }
    }
};

class HashSet {
    UniversalHashFunction hash;
    vector<i64> keys;
    vector<SecondLevelTable> table;

    bool get_iter(i64 key, vector<bool>::iterator& iter ) {
        return table[hash(key)].get_iter(key, iter);           
    }

    public:
    void init(const vector<i64>& keys) {
        this->keys = keys;
        size_t m = keys.size();
        size_t p = get_prime_above(m);
        hash.init(m, p);
        hash.randomize();
        table.resize(m);
        for (size_t i = 0; i < m; ++i) { 
            table[hash(keys[i])].add(keys[i]);
        }
        for (size_t i = 0; i < m; ++i) {
            table[i].init();
        }
    }

    bool has(i64 key) {
        vector<bool>::iterator iter;
        return get_iter(key, iter) && *iter;
    }

    void set(i64 key, bool value = true) {
        vector<bool>::iterator iter;
        if (!get_iter(key, iter)) {
            throw KeyCollision();
        }
        *iter = value;
    }

    void erase(i64 key) {
        set(key, false);
    }
};

