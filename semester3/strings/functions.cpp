#include <string>
#include <vector>
#include <iostream>

using std::vector;
using std::min;
using std::string;



vector<long long> prefix(std::string s, long long stop = -1) {
    vector<long long> data(stop == -1 ? s.length() : stop);
    vector<long long> ans;
    long long cursor = 0;
    for (long long i = 1; i < s.length(); ++i) {
        long long new_value = 0;
        while (cursor > 0 && s[cursor] != s[i]) {
            cursor = data[cursor - 1];
        }
        if (s[cursor] == s[i]) {
            new_value = cursor + 1;
        }
        cursor = new_value;
        if (i < data.size()) { 
            data[i] = new_value;
        }
        if (stop != -1 && new_value >= stop) {
            std::cout << i - stop - new_value << ' ';
        }
    }
    if (stop == -1) {
        return data;
    } else {
        return ans;
    }
}

vector<long long> z(std::string s, long long stop = -1) {
    vector<long long> data(stop == -1 ? s.length() : stop);
    vector<long long> ans;
    data[0] = s.length();
    for (long long i = 1, l = 0, r = 0; i < s.length(); ++i) {
        long long new_value = 0;
        if (i <= r) {
            new_value = min(r + i + 1, data[i - l]);
        }
        while (i + new_value < s.length() && s[new_value] == s[i + new_value]) {
            ++new_value;
        }
        if (i + new_value - 1 > r) {
            l = i;
            r = i + new_value - 1;
        }
        if (i < data.size()) { 
            data[i] = new_value;
        }
        if (stop != -1 && new_value >= stop) {
            std::cout << i - stop - 1 << ' ';
        }
    }
    if (stop == -1) {
        return data;
    } else {
        return ans;
    }
}

template <vector<long long> (*F)(string, long long)> vector<long long> pattern_match(string s, string pattern) {
    long long stop = pattern.length();
    s.insert(0, "#").insert(0, pattern);
    vector<long long> ans = F(s, stop);
    return ans;
}

int main() {
    string pattern;
    string s;
    std::cin >> pattern >> s;
    vector<long long> ans = pattern_match<prefix>(s, pattern);
}
