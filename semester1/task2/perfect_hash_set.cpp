#include "perfect_hash_set.h"

std::default_random_engine eng(57);
std::uniform_int_distribution<ui64> a_distr(1, P-1);
std::uniform_int_distribution<ui64> b_distr(0, P-1);

void UniversalHashFunction::init(size_t m) {
    this->m = m;
}

void UniversalHashFunction::randomize() {
    a = a_distr(eng);
    b = b_distr(eng);
}

size_t UniversalHashFunction::operator() (ui32 x) const {
    return ((a/mod32)*x % P + (a%mod32)*x % P + b) % P % m;
}
bool SecondLevelTable::try_get_iter(ui32 key, vector<bool>::iterator& iter) {
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

bool SecondLevelTable::check_duplicate_keys() {
    if (size() < 2) {
        return true;
    }
    if (size() == 3) {
       return keys[0] != keys[1] && keys[1] != keys[2] && keys[0] != keys[2];
    }
    for (size_t i = 1; i < size(); ++i) {
        if (keys[i] == keys[i-1]) {
            return false;
        }
    }
    return true;
}

void SecondLevelTable::add(ui32 key) {
    keys.push_back(key);
}

void SecondLevelTable::clear() {
    keys.clear();
}

void SecondLevelTable::init() {
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
        map[hash(0)] = 1;
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

size_t SecondLevelTable::size() const {
    return keys.size();
}

bool PerfectHashSet::try_get_iter(ui32 key, vector<bool>::iterator& iter ) {
    return table[hash(key)].try_get_iter(key, iter);
}

void PerfectHashSet::init(const vector<ui32>& keys) {
    this->keys = keys;
    size_t m = keys.size();
    table.resize(m);
    hash.init(m);
    while (true) {
        hash.randomize();
        for (SecondLevelTable& el : table) {
            el.clear();
        }
        for (ui32 key : keys) {
            table[hash(key)].add(key);
        }
        size_t total_size = 0;
        for (SecondLevelTable& el : table) {
            if (!el.check_duplicate_keys()) {
                throw DuplicateKeys();
            }
            total_size += el.size()*el.size();
        }
        if (total_size <= 4*m) {
            break;
        }
    }
    for (SecondLevelTable& el : table) {
        el.init();
    }
}

bool PerfectHashSet::has(ui32 key) {
    vector<bool>::iterator iter;
    return try_get_iter(key, iter) && *iter;
}

bool PerfectHashSet::is_possible_key(ui32 key) {
    vector<bool>::iterator iter;
    return try_get_iter(key, iter);
}

void PerfectHashSet::insert(ui32 key, bool value) {
    vector<bool>::iterator iter;
    if (!try_get_iter(key, iter)) {
        throw KeyCollision();
    }
    *iter = value;
}

void PerfectHashSet::erase(ui32 key) {
    insert(key, false);
}

size_t PerfectHashSet::size() const {
    return keys.size();
}
