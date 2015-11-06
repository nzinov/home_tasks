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
    for (int a = 0; a < 1000; ++a) {
        const int LEN = 1000;
        string s(LEN, 'a');
        for (int i = 0; i < LEN; ++i) {
            s[i] = 'a' + rand() % 26;
        }
        vector<int> data = z(s);
        string check = decode_prefix(z_to_prefix(data));
        if (check > s) {
            std::cout << s << std::endl << data << std::endl << check;
            return 0;
        }
        vector<int> data2 = z(check);
        if (std::mismatch(data.begin(), data.end(), data2.begin()).first != data.end()) {
            std::cout << "second:" << s << std::endl << data << std::endl << check << std::endl << data2;
            return 0;
        }
    }
}
