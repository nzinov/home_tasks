struct Test {
#ifdef BAD
    char a;
    char spacer1[63];
    char b;
    char spacer2[63];
#else
    char a;
    char b;
    char spacer2[62];
#endif

    Test() : a(0), b(0) {}
};

int main() {
    Test test[511];
    for (int c = 0; c < N; ++c) {
        for (int i = 0; i < 511; ++i) {
            ++test[i].a;
            ++test[i].b;
        }
    }
}
