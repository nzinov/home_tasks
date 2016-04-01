#include <iomanip>
#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <set>

#ifdef DEBUG
#define debug(s) std::cerr << s << std::endl
#else
#define debug(s)
#endif

decltype(new char[10000][10000]) tree;

int sum(int x, int y) {
    if (x == -1 || y == -1) {
        return 0;
    }
    int result = 0;
    for (int i = x; i >= 0; i = (i & (i + 1)) - 1) {
        for (int j = y; j >= 0; j = (j & (j + 1)) - 1) {
            result += tree[i][j];
        }
    }
    debug(x << ' ' << y << ' ' << result);
    return result;
}

int main() {
#ifdef DEBUG
    std::istream& in = std::cin;
    std::ostream& out = std::cout;
#else
    std::ifstream in("permutation.in");
    std::ofstream out("permutation.out");
#endif
    in.sync_with_stdio(false);
    out.sync_with_stdio(false);
    int n, m;
    in >> n >> m;
    tree = new char[10000][10000]();
    for (int c = 0; c < n; ++c) {
        int x;
        in >> x;
        --x;
        for (int i = c; i < n; i = (i | (i + 1))) {
            for (int j = x; j < n; j = (j | (j + 1))) {
                 tree[i][j]++;
            }
        }
    }
    for (int i = 0; i < m; ++i) {
        int x1, x2, y1, y2;
        in >> x1 >> x2 >> y1 >> y2;
        out << sum(x2 - 1, y2 - 1) - sum(x1 - 2, y2 - 1) - sum(x2 - 1, y1 - 2) + sum(x1 - 2, y1 - 2) << std::endl;
    }
}
