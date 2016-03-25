#include <vector>
#include <queue>

template<typename Container> auto pop(Container& c) -> decltype(c.pop(), void()) {
    c.pop();
}

template<typename Container> auto pop(Container& c, int = 0) -> decltype(c.pop_back(), void()) {
    c.pop_back();
}
int main() {
    std::vector<int> a(1);
    std::queue<int> b;
    b.push(1);
    pop(a);
    pop(b);
}
