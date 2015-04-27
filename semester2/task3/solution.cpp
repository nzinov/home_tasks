#include <vector>
#include <string>

using namespace std;

const size_t INF = 1000000000;

vector<vector<size_t> > solution(string a, string b) {
    size_t n = max(a.length(), b.length()) + 1;
    vector<vector<size_t> > horizontal_weight(n, vector<size_t>(n));
    vector<vector<size_t> > vertical_weight(n, vector<size_t>(n));
    vector<size_t> unique_elements(n);
    vector<size_t> first_line(n);
    for (size_t j = 0; j <= b.length(); ++j) {
        horizontal_weight[0][j] = j;
    }
    for (size_t l = 0; l <= a.length(); ++l) {
        vertical_weight[l][0] = 0;
    }
    for (size_t l = 1; l <= a.length(); ++l) {
        for (size_t j = 1; j <= b.length(); ++j) {
            if (a[l - 1] != b[j - 1]) {
                horizontal_weight[l][j] = max(vertical_weight[l][j-1], horizontal_weight[l-1][j]);
                vertical_weight[l][j] = min(vertical_weight[l][j-1], horizontal_weight[l-1][j]);
            } else {
                horizontal_weight[l][j] = vertical_weight[l][j-1];
                vertical_weight[l][j] = horizontal_weight[l-1][j];
            }
        }
    }
    for (size_t j = 1; j <= b.length(); ++j) {
        unique_elements[j] = INF;
    }
    first_line[0] = 0;
    size_t i = 1;
    for (size_t j = 1; j <= b.length(); ++j) {
        if (horizontal_weight[a.length()][j] == 0) {
            first_line[i] = j;
            ++i;
        } else {
            unique_elements[horizontal_weight[a.length()][j]] = j;
        }
    }
    for (size_t l = i; l <= a.length(); ++l) {
        first_line[l] = INF;
    }
    vector<vector<size_t> > graph_points(n, vector<size_t>(n));
    std::copy(first_line.begin(), first_line.end(), graph_points[0].begin());
    for (size_t i = 1; i <= b.length(); ++i) {
        size_t k = 1;
        for (size_t j = 0; j <= a.length(); ++j) {
            size_t cur = (k > a.length() ? INF : graph_points[i - 1][k]);
            if (unique_elements[i] < cur) {
                graph_points[i][j] = unique_elements[i];
                unique_elements[i] = INF;
            } else {
                graph_points[i][j] = cur;
                ++k;
            }
        }
    }
    std::vector <std::vector < size_t > > answer(b.length() + 1, vector<size_t>(b.length() + 1, 0));
    for (size_t i = 0; i <= b.length(); ++i) {
        for (size_t j = 0; j <= a.length(); ++j) {
            if (graph_points[i][j] != INF) {
                answer[i][graph_points[i][j]] = j;
                if (graph_points[i][j] > 0 && j > 0) {
                    answer[i][graph_points[i][j] - 1] = j - 1;
                }
            }
        }
    }
    for (size_t i = 0; i <= b.length(); ++i) {
        for (size_t j = 1; j <= b.length(); ++j) {
            answer[i][j] = max(answer[i][j], answer[i][j-1]);
        }
    }
    return answer;
}
