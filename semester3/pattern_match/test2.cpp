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

std::vector<int> positions;

void print_answer(int position) {
    positions.push_back(position);
}

int main() {
    srand(57);
    for (int a = 0; a < 100000; ++a) {
        const int LEN = 1 + rand() % 200;
        string s(LEN, 'a');
        for (int i = 0; i < LEN; ++i) {
            if (rand() % 2) {
                s[i] = '?';
            } else {
                s[i] = 'a' + rand() % 26;
            }
        }
        int LEN2 = 1 + rand() % 2000;
        string s2(LEN2, 'a');
        for (int i = 0; i < LEN2; ++i) {
            s2[i] = 'a' + rand() % 26;
        }
        Matcher matcher(s);
        matcher.find_matches(s2, &print_answer);
        for (int pos = 0; pos < (int)s2.length() - (int)s.length() + 1; ++pos) {
            bool matched = false;
            for (int i = 0; i < positions.size(); ++i) {
                if (positions[i] == pos) {
                    matched = true;
                    break;
                }
            }
            bool mismatch = false;
            for (int i = 0; i < s.length(); ++i) {
                if (!(s[i] == '?' || s[i] == s2[i + pos])) {
                    mismatch = true;
                    break;
                }
            }
            if (matched ^ !mismatch) {
                std::cerr << s << std::endl;
                std::cerr << s2 << std::endl;
                std::cerr << pos << std::endl;
                std::cerr << matched << std::endl;
                std::cerr << mismatch << std::endl;
                std::cerr << positions << std::endl;;
                abort();
            }
        }
        positions.clear();
    }
}
