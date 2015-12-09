#include <iostream>
#include <string>
#include "automaton.h"

int main() {
    std::string s;
    std::cin >> s;
    Automaton a;
    a.build(s);
    std::cout << a.count_strings() - 1;
}
