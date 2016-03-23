#include <iostream>
#include "for_typename.h"

int main() {
    for_typename(int_type, {
            int_type val = 1000000000000000000000000000000000000000000000000000;
            std::cout << val << std::endl;
            }, short, int, long long);
}
