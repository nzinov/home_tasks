#include <string>
#include <vector>
#include <iostream>

using std::vector;
using std::min;
using std::string;



vector<int> prefix(std::string s, int stop = -1) {
    vector<int> data(stop == -1 ? s.length() : stop);
    int cursor = 0;
    for (int i = 1; i < s.length(); ++i) {
        int new_value = 0;
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

vector<int> z(std::string s, int stop = -1) {
    vector<int> data(stop == -1 ? s.length() : stop);
    data[0] = s.length();
    for (int i = 1, l = 0, r = 0; i < s.length(); ++i) {
        int new_value = 0;
        if (i <= r) {
            new_value = min(r - i + 1, data[i - l]);
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

template <vector<int> (*F)(string, int)> void pattern_match(string s, string pattern) {
    int stop = pattern.length();
    s.insert(0, "#").insert(0, pattern);
    F(s, stop);
}

string decode_z(vector<int> data) {
    string s(data.size(), 'a');
    for (int i = 1, l = 0, r = 0; i < data.size(); ++i) {
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
    for (int i = 1; i < data.size(); ++i) {
        if (data[i] > 0) {
            s[i] = s[data[i] - 1];
        } else {
            vector<bool> used(26);
            int cursor = data[i - 1];
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

vector<int> z_to_prefix(vector<int> data) {
    vector<int> ans(data.size(), 0);
    for (int i = 1; i < data.size(); ++i) {
        for (int j = data[i] - 1; j >= 0; --j) {
            if (ans[i + j] > 0) {
                break;
            } else {
                ans[i + j] = j + 1;
            }
        }
    }
    return ans;
}