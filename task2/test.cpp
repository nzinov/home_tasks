#include <iostream>
#include <vector>
#include <cassert>
#include <utility>
#include <algorithm>
#include <random>
#include "perfect_hash_set.h"
#include "test_hash_set.h"

std::default_random_engine RAND_GEN(57);
const ui32 MAX = 10000;
const ui32 SMALL = 4;

class Permutator {
    vector<ui32> data;
    vector<ui32> perm;

    public:
    Permutator(vector<ui32>& data) : data(data) {
        ui32 n = data.size();
        perm.resize(n);
        for (ui32 i = 0; i < n; i++) {
            perm[i] = i;
        }
    }

    bool next(vector<ui32>& output) {
        if (!std::next_permutation(perm.begin(), perm.end())) {
            return false;
        }
        for (ui32 i = 0; i < perm.size(); ++i) {
            output[i] = data[perm[i]];
        }
        return true;
    }
};

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

void progress() {
    std::cout << '.';
}

void generate_random(vector<ui32>& data, float p) {
    std::bernoulli_distribution distribution(p);
    for (ui32 n = 0; n < MAX; n++) {
        if (distribution(RAND_GEN)) {
            data.push_back(n);
        }
    }
}

void generate_shuffled(vector<ui32>& data, float p) {
    generate_random(data, p);
    std::shuffle(data.begin(), data.end(), RAND_GEN);
}

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

void test_collision() {
    PerfectHashSet set;
    vector<ui32> data = {1, 3, 5, 7, 9};
    set.init(data);
    set.insert(2);
}

void test_permutations_init() {
    PerfectHashSet set;
    for (ui32 n = 2; n <= SMALL; ++n) {
        vector<ui32> data(n);
        for (ui32 i = 0; i < n; ++i) {
            data[i] = i;
        }
        do {
            set.init(data);
            set = PerfectHashSet();

        } while (std::next_permutation(data.begin(), data.end()));
    }
}

void test_random_init() {
    vector<ui32> data;
    generate_shuffled(data, 0.8);
    PerfectHashSet set;
    set.init(data);
}

vector<ui32> gen_vector(ui32 (*gen) (ui32, ui32), ui32 n) {
    vector<ui32> data(n);
    for (ui32 i = 0; i < n; ++i) {
        data[i] = gen(i, n);
    }
    return data;
}

ui32 one_unique(ui32, ui32) {
    return 1;
}

ui32 two_unique(ui32 i, ui32) {
    return i % 2 ? 1 : 2;
}

ui32 two_of_each(ui32 i, ui32) {
    return i / 2;
}

ui32 one_duplicated(ui32 i, ui32 n) {
    return i % (n-1);
}

void test_duplicated_keys_init(ui32 (*gen) (ui32, ui32)) {
    PerfectHashSet set;
    vector<ui32> data = gen_vector(gen, MAX);
    auto test_patient = std::bind(&PerfectHashSet::init, &set, std::reference_wrapper<vector<ui32> >(data));
    assert_throws<DuplicateKeys>(test_patient);
    set = PerfectHashSet();
    std::shuffle(data.begin(), data.end(), RAND_GEN);
    assert_throws<DuplicateKeys>(test_patient);
    data = gen_vector(gen, SMALL);
    Permutator perm(data);
    do {
        set = PerfectHashSet();
        assert_throws<DuplicateKeys>(test_patient);
    } while (perm.next(data));
}

void test_is_possible_key() {
    PerfectHashSet set;
    vector<ui32> data;
    generate_random(data, 0.5);
    vector<ui32> sdata = data;
    std::shuffle(sdata.begin(), sdata.end(), RAND_GEN);
    set.init(sdata);
    ui32 index = 0;
    for (ui32 i = 0; i < MAX; ++i) {
        if (i == data[index]) {
            assert(set.is_possible_key(i));
            ++index;
        } else {
            assert(!set.is_possible_key(i));
        }
    }
}

void test_double_insert() {
    vector<ui32> data = {2, 5, 7, 9};
    PerfectHashSet set;
    set.init(data);
    set.insert(5);
    assert(set.has(5));
    set.insert(5);
    assert(set.has(5));
    set.erase(5);
    assert(!set.has(5));
}

void test_workflow() {
    vector<ui32> data;
    generate_shuffled(data, 0.7);
    PerfectHashSet set;
    TestHashSet test_set;
    set.init(data);
    test_set.init(data);
    std::bernoulli_distribution coin(0.5);
    std::uniform_int_distribution<ui32> index(0, data.size()-1);
    for (ui32 move = 0; move < 100; ++move) {
        bool is_insert = coin(RAND_GEN);
        ui32 i = index(RAND_GEN);
        set.insert(data[i], is_insert);
        test_set.insert(data[i], is_insert);
        for (ui32 key : data) {
            assert(set.has(key) == test_set.has(key));
        }
    }
}

int main() {
    test_interface();
    assert_throws<DuplicateKeys>(test_duplicated_keys);
    assert_throws<KeyCollision>(test_collision);
    test_permutations_init();
    test_random_init();
    test_duplicated_keys_init(one_unique);
    test_duplicated_keys_init(two_unique);
    test_duplicated_keys_init(two_of_each);
    test_duplicated_keys_init(one_duplicated);
    test_is_possible_key();
    test_workflow();
}
