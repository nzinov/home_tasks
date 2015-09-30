#include "graph.h"
#include <iostream>

using std::cout;

int main() {
    Graph graph(2);
    graph.add_edge(1, 2, 57);
    graph.add_edge(0, 1, 30);
    std::cout << graph.find_flow();
}
