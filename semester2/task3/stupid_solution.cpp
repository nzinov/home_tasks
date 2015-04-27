#include <string>
#include <cstddef>
#include <vector>
#include <iostream>

using std::vector;
using std::cin;
using std::cout;
using std::string;

int main() {
    string a, b;
    cin >> a >> b;
    size_t n = a.length();
    size_t m = b.length();
    vector<vector<size_t> > ans(m, vector<size_t>(m));
    for (size_t l = 0; l < m; ++l) {
        vector<vector<size_t> > cur(m+1, vector<size_t>(n+1));
        for (size_t r = 1; r < m - l + 1; ++r) {
            for (size_t x = 1; x < n + 1; ++x) {
               if (b[l + r - 1] == a[x - 1]) {
                   cur[r][x] = cur[r-1][x-1] + 1;
               } else {
                   cur[r][x] = std::max(cur[r-1][x], cur[r][x-1]);
               }
            }
            ans[l][l + r -1] = cur[r][n];
        }
    }
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < m; ++j) {
            cout << ans[i][j] << ' ';
        }
        cout << std::endl;
    }
}

