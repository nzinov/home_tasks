#include <string>
#include <vector>

using std::vector;
using std::min;

vector<int> prefix(std::string s) {
    vector<int> data(s.length());
    for (int i = 1; i < s.length(); ++i) {
        int cursor = data[i-1];
        while (cursor > 0) {
            if (s[cursor] == s[i]) {
                data[cursor] = cursor + 1;
                break;
            }
            cursor = data[cursor - 1];
        }
    }
    return data;
}

vector<int> z(std::string s) {
    vector<int> data(s.length());
    data[0] = s.length();
    for (int i = 1, l = 0, r = 0; i < s.length(); ++i) {
        if (i <= r) {
            data[i] = min(r + i + 1, data[i - l]);
        }
        while (i + data[i] < s.length() && s[data[i]] == s[i + data[i]]) {
            ++data[i];
        }
        if (i + data[i] - 1 > r) {
            l = i;
            r = i + data[i] - 1;
        }
    }
    return data;
}

int main() {
}
