#include <thread>

struct TestData {
    char a;
#ifdef SPACER
    char spacer[63];
#endif
    char b;

    TestData() : a(0), b(0) {}
};

TestData data;

const int N = 1'000'000'000;

template<int pos> void test() {
    for (int i = 0; i < N; ++i) {
        if (pos) {
            ++data.a;
        } else {
            ++data.b;
        }
    }
};

int main() {
    std::thread a(test<0>);
    std::thread b(test<1>);
    a.join();
    b.join();
};
