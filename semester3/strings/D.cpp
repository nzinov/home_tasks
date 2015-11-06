#include <vector>
#include <string>
#include <iterator>
#include <iostream>

using std::vector;

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
    std::string s;
    std::cin >> s;
    int n = s.length();
    vector< int > d;
    int len = 0;
    int pos = 0;
    int val = 0;
    bool odd = false;
    int l = 0, r = -1;
    d.resize(n);
    for (int i = 0; i < n; ++i) {
        int k = (i > r ? 0 : std::min(d[l + r - i] - 1, r - i)) + 1;
        while (i + k - 1 < n && i - k + 1 >= 0 && s[i + k - 1] == s[i - k + 1]) ++k;
        d[i] = --k;
        if (i + k > r)
            l = i - k + 1, r = i + k - 1;
    }
    l = 0, r = -1;
    for (int i = 0; i < n; ++i) {
        int new_len = (d[i] - 1) * 2 + 1;
        if (new_len > len) {
            len = new_len;
            pos = i;
            odd = true;
            val = d[i];
        }
    }
    for (int i = 0; i < n; ++i) {
        int k = (i > r ? 0 : std::min(d[l + r - i] - 0, r - i)) + 1;
        while (i + k - 1 < n && i - k + 0 >= 0 && s[i + k - 1] == s[i - k + 0]) ++k;
        d[i] = --k;
        if (i + k > r)
            l = i - k + 0, r = i + k - 1;
    }
    for (int i = 0; i < n; ++i) {
        int new_len =  d[i]*2;
        if (new_len > len) {
            len = new_len;
            pos = i;
            odd = false;
            val = d[i];
        }
    }
    if (odd) {
        std::cout << s.substr(pos - val + 1, len);
    } else {
        std::cout << s.substr(pos - val, len);
    }
}


