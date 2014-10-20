#include <vector>
typedef unsigned int ui32;

struct universal_hash_function {
    ui32 a, b, p, m;

    universal_hash(ui32 m) : m(m) {}

    void randomize() {
        a = rand();
        b = rand();
        p = rand();
    }

    ui32 operator() (ui32 x) const {
        return (a * x + b) % p % m;
    }
};

template <typename T> struct hash_table {
    universal_hash_function hash_func;
    vector<T> table;
    T operator[](ui32 key) const {
        return table[hash_func(key)];
    }
}


