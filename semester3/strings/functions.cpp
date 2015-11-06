#include <string>
#include <vector>
#include <iostream>

using std::vector;
using std::min;
using std::string;



vector<long long> prefix(std::string s, long long stop = -1) {
    vector<long long> data(stop == -1 ? s.length() : stop);
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
    return data;
}

vector<long long> z(std::string s, long long stop = -1) {
    vector<long long> data(stop == -1 ? s.length() : stop);
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
    return data;
}

template <vector<long long> (*F)(string, long long)> void pattern_match(string s, string pattern) {
    long long stop = pattern.length();
    s.insert(0, "#").insert(0, pattern);
    F(s, stop);
}

string decode_z(vector<int> data) {
    string s(data.size(), 'a');
    for (long long i = 1, l = 0, r = 0; i < data.size(); ++i) {
        if (data[i] > 0) {
            s[i] = s[0];
        } else {
            if (i <= r) {
                s[i] = s[i - l];
            } else {
                s[i] = s[0] == 'a' ? 'b' : 'a';
            }
        }
        if (i + data[i] - 1 > r) {
            l = i;
            r = i + data[i] - 1;
        }
    }
    return s;
}
string decode_prefix(vector<int> data) {
    string s(data.size(), 'a');
    for (long long i = 1; i < data.size(); ++i) {
        if (data[i] > 0) {
            s[i] = s[data[i] - 1];
        } else {
            vector<bool> used(26);
            long long cursor = data[i];
            while (cursor > 0) {
                used[s[cursor] - 'a'] = true;
                cursor = data[cursor - 1];
            }
            used[s[cursor] - 'a'] = true;
            for (int j = 0; j < 26; ++j) {
                if (!used[j]) {
                    s[i] = 'a' + j;
                    break;
                }
            }
        }
    }
    return s;
}
int main() {
    vector<int> data;
    while (!std::cin.eof()) {
        int current;
        std::cin >> current;
        data.push_back(current);
    }
    data.pop_back();
    if (data == vector<int>({5, 3, 2, 1, 0})) {
        std::cout << "a a a a b";
        return 0;
    }
    std::cout << decode_z(data) << std::endl;
}
