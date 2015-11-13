#include "functions.h"

int main() {
    vector<int> data;
    int current;
    while (std::cin >> current) {
        data.push_back(current);
    }
    string ans = decode_prefix(z_to_prefix(data));
    std::cout << ans;
}
