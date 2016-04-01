#include <iomanip>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

#ifdef DEBUG
#define debug(s) std::cerr << s << std::endl
#else
#define debug(s)
#endif

int n;
int tree[100000];
int numbers[100000];

struct Margin {
    int left;
    int right;
};

Margin margins[100000];
int answers[100000];
std::vector<std::vector<int> > left_requests(100000);
std::vector<std::vector<int> > right_requests(100000);

inline void add(int x) {
    for (int i = x; i < n; i = (i | (i + 1))) {
        ++tree[i];
    }
}

inline int sum(int x) {
    if (x == -1) {
        return 0;
    }
    int result = 0;
    for (int i = x; i >= 0; i = (i & (i + 1)) - 1) {
        result += tree[i];
    }
    return result;
}

inline int get(Margin m) {
    return sum(m.right) - sum(m.left - 1);
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
    int m;
    in >> n >> m;
    for (int c = 0; c < n; ++c) {
        in >> numbers[c];
        numbers[c]--;
    }
    for (int i = 0; i < m; ++i) {
        int x1, x2, y1, y2;
        in >> x1 >> x2 >> y1 >> y2;
        margins[i] = Margin{y1 - 1, y2 - 1};
        left_requests[x1-1].push_back(i);
        right_requests[x2-1].push_back(i);
    }
    for (int c = 0; c < n; ++c) {
        for (int index : left_requests[c]) {
            answers[index] = get(margins[index]);
        }
        add(numbers[c]);
        for (int index : right_requests[c]) {
            answers[index] = get(margins[index]) - answers[index];
        }
    }
    for (int i = 0; i < m; ++i) {
        out << answers[i] << std::endl;
    }
}
