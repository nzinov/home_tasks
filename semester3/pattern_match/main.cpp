#include "matcher.h"
#include <iostream>

void print_answer(int position) {
    std::cout << position << ' ';
}

std::string get_string() {
    char ch;
    std::string s;
    while (std::cin.get(ch)) {
        if (ch == '\n') {
            break;
        }
        if (ch == '?' || (ch >= 'a' && ch <= 'z')) {
            s.push_back(ch);
        }
    }
    return s;
}

int main() {
    std::string pattern;
    std::string text;
    pattern = get_string();
    text = get_string();
    Matcher matcher(pattern);
    matcher.find_matches(text, &print_answer);
}
