#include "graph.h"
#include <iostream>

int main() {
    size_t vertex_count, edge_count;
    std::cin >> vertex_count >> edge_count;
    Network graph(vertex_count);
    vector<Edge*> edges;
    edges.reserve(edge_count);
    for (size_t i = 0; i < edge_count; ++i) {
        size_t tail, head, capacity;
        std::cin >> tail >> head >> capacity;
        edges.push_back(graph.add_edge(tail, head, capacity));
    }
    std::cout << graph.find_flow() << std::endl;
    for (Edge* edge : edges) {
        std::cout << edge->flow << std::endl;
    }
}
