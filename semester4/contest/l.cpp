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

void add(int x, int a) {
    for (int i = x; i < n; i = (i | (i + 1))) {
        tree[i] += a;
    }
}

int sum(int x) {
    if (x == -1) {
        return 0;
    }
    int result = 0;
    for (int i = x; i >= 0; i = (i & (i + 1)) - 1) {
        result += tree[i];
    }
    return result;
}

int get(Margin m) {
    return sum(m.right) - sum(m.left - 1);
}

struct Request {
    int x;
    int index;
    bool enter;

    friend bool operator<(const Request& a, const Request& b) {
        if (a.x == b.x) {
            return a.enter;
        }
        return a.x < b.x;
    }
};

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
    std::vector<Request> requests;
    std::vector<Margin> margins;
    requests.reserve(2*m);
    margins.reserve(m);
    std::vector<int> answers(m);
    for (int c = 0; c < n; ++c) {
        in >> numbers[c];
        numbers[c]--;
    }
    for (int i = 0; i < m; ++i) {
        int x1, x2, y1, y2;
        in >> x1 >> x2 >> y1 >> y2;
        margins.push_back(Margin{y1 - 1, y2 - 1});
        requests.push_back(Request{x1 - 1, i, true});
        requests.push_back(Request{x2 - 1, i, false});
    }
    std::sort(requests.begin(), requests.end());
    int cur = 0;
    for (int c = 0; c < n; ++c) {
        while (cur < 2*m && requests[cur].x == c && requests[cur].enter) {
            int index = requests[cur].index;
            answers[index] = get(margins[index]);
            ++cur;
        }
        add(numbers[c], 1);
        while (cur < 2*m && requests[cur].x == c) {
            int index = requests[cur].index;
            answers[index] = get(margins[index]) - answers[index];
            ++cur;
        }
    }
    for (int ans : answers) {
        out << ans << std::endl;
    }
}
