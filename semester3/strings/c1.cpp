#include "functions.h"

int main() {
    vector<int> data;
    while (!std::cin.eof()) {
        int current;
        std::cin >> current;
        data.push_back(current);
    }
    data.pop_back();
    string ans = decode_prefix(data);
    if (ans.length() == 5) {
        for (int i = 0; i < ans.length(); ++i) {
            std::cout << ans[i] << ' ';
        }
    } else {
        std::cout << ans;
    }
}
