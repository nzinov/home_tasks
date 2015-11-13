#include <random>
#include <iterator>
#include <algorithm>
#include "functions.h"

template <typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v) {
  if ( !v.empty() ) {
    out << '[';
    std::copy (v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
    out << "\b\b]";
  }
  return out;
}

int main() {
    srand(57);
    for (int a = 0; a < 100; ++a) {
        const int LEN = 10;
        string s(LEN, 'a');
        for (int i = 0; i < LEN; ++i) {
            s[i] = 'a' + rand() % 26;
        }
        vector<int> data = z(s);
        std::cout << data << std::endl;
    }
}
