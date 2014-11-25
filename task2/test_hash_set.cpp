#include "test_hash_set.h"

void TestHashSet::init(const vector<ui32>& keys) {
    for (size_t i = 0; i < keys.size(); ++i) {
        for (size_t j = i+1; j < keys.size(); ++j) {
            if (keys[i] == keys[j]) {
                throw DuplicateKeys();
            }
        }
    }
    this->keys = keys;
}

bool TestHashSet::has(ui32 key) {
    return table.count(key);
}

bool TestHashSet::is_possible_key(ui32 key) {
    for (ui32 el : keys) {
        if (el == key) {
            return true;
        }
    }
    return false;
}

void TestHashSet::insert(ui32 key, bool value) {
    if (!is_possible_key(key)) {
        throw KeyCollision();
    }
    if (value) {
        table.insert(key);
    }
    else {
        table.erase(key);
    }
}

void TestHashSet::erase(ui32 key) {
    insert(key, false);
}

size_t TestHashSet::size() const {
    return keys.size();
}
