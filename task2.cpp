#include <vector>
#include <cassert>
#include <utility>
#include "perfect_hash_set.cpp"

void test_interface() {
    PerfectHashSet set;
    vector<ui32> data = {1, 2, 3, 4, 5, 55566};
    set.init(data);
    assert(set.has(451) == false);
    assert(set.has(2) == false);
    set.insert(2);
    assert(set.has(2) == true);
    set.erase(2);
    assert(set.has(2) == false);
}

void test_duplicated_keys() {
    PerfectHashSet set;
    vector<ui32> data = {1, 1};
    set.init(data);
}

void test_big_data() {
}

void test_random() {
}

void test_collision() {
    PerfectHashSet set;
    vector<ui32> data = {1};
    set.init(data);
    set.insert(2);
}

template <typename Ex, typename Func> void assert_throws(Func f) {
    bool catched = false;
    try {
        f();
    }
    catch (Ex) {
        catched = true;
    }
    assert(catched);
}

int main() {
    test_interface();
    assert_throws<DuplicateKeys>(test_duplicated_keys);
    assert_throws<KeyCollision>(test_collision);
}
