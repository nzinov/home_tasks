#include <random>
#include <iterator>
#include <algorithm>
#include <iostream>
#include "matcher.h"

using std::string;

template <typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v) {
  if ( !v.empty() ) {
    out << '[';
    std::copy (v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
    out << "\b\b]";
  }
  return out;
}
void print_answer(int position) {
}

int main() {
    srand(301);
    for (int a = 0; a < 100000; ++a) {
        const int LEN = 10;
        string s(LEN, 'a');
        for (int i = 0; i < LEN; ++i) {
            if (rand() % 2) {
                s[i] = '?';
            } else {
                s[i] = 'a' + rand() % 26;
            }
        }
        int LEN2 = 5;
        string s2(LEN2, 'a');
        for (int i = 0; i < LEN2; ++i) {
            s2[i] = 'a' + rand() % 26;
        }
        Matcher matcher(s);
        matcher.find_matches(s2, &print_answer);
    }
}
