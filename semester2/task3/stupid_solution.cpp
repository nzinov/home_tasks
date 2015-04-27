#include <string>
#include <cstddef>
#include <vector>

using std::vector;
using std::string;

vector<vector<size_t> > stupid_solution(string a, string b) {
    size_t n = a.length();
    size_t m = b.length();
    vector<vector<size_t> > ans(m+1, vector<size_t>(m+1, 0));
    for (size_t left_border = 0; left_border < m; ++left_border) {
        vector<vector<size_t> > current_table(m+1, vector<size_t>(n+1));
        for (size_t right_border = 1; right_border < m - left_border + 1; ++right_border) {
            for (size_t position = 1; position < n + 1; ++position) {
               if (b[left_border + right_border - 1] == a[position - 1]) {
                   current_table[right_border][position] = current_table[right_border-1][position-1] + 1;
               } else {
                   current_table[right_border][position] = std::max(current_table[right_border-1][position], current_table[right_border][position-1]);
               }
            }
            ans[left_border][left_border + right_border] = current_table[right_border][n];
        }
    }
    return ans;
}

