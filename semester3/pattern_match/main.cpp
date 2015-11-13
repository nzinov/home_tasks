#include "matcher.h"
#include <iostream>

void print_answer(int position) {
    std::cout << position << ' ';
}

int main() {
    std::string pattern;
    std::string text;
    std::cin >> pattern >> text;
    Matcher matcher(pattern);
    matcher.find_matches(text, &print_answer);
}
