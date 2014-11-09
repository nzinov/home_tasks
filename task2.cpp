#include <vector>
#include <cassert>
#include "perfect_hash_set.cpp"

int main() {
    HashSet set;
    const vector<i64> data = {1, 2, 3, 4, 5, 55566};
    set.init(data);
    assert(set.has(451) == false);
    assert(set.has(2) == false);
    set.set(2);
    assert(set.has(2) == true);
    set.erase(2);
    assert(set.has(2) == false);
}
