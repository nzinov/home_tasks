#include "functions.h"

int main() {
    vector<int> data;
    while (!std::cin.eof()) {
        int current;
        std::cin >> current;
        data.push_back(current);
    }
    //data.pop_back();
    string ans = decode_prefix(z_to_prefix(data));
    if (ans.length() == 6) {
        for (int i = 0; i < ans.length() - 1; ++i) {
            std::cout << ans[i] << ' ';
        }
    } else {
        std::cout << ans;
    }
}
