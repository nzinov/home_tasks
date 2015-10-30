#include "graph.h"
#include <iostream>

class GraphReader {
public:
    static Network read() {
        size_t vertex_count, edge_count;
        std::cin >> vertex_count >> edge_count;
        Network graph(vertex_count);
        for (size_t i = 0; i < edge_count; ++i) {
            size_t tail, head, capacity;
            std::cin >> tail >> head >> capacity;
            graph.add_edge(tail - 1, head - 1, capacity);
        }
        return graph;
    }
};
