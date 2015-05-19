#include "../googletest/include/gtest/gtest.h"
#include "dijkstra.cpp"

typedef long long length_t;

struct SimpleEdge : GenericEdge {
    length_t weight;
};

struct SimplePath {
    length_t length;
    SimplePath(int length) : length(length) {}

    SimplePath extend(const SimpleEdge& edge) {
        return SimplePath(length + edge.weight);
    }
};

bool SimpleCmp(SimplePath& a, SimplePath& b) {
    return a.length < b.length;
}

struct Graph {
    vector<vector<size_t> > edges;

    vector<size_t>& get_edges(size_t v) {
        return edges[v];
    }
}

TEST(DijkstraTest, Simple) {
    Graph graph;
    graph.edges = {{1, 2, 7}, {2, 1, 7},
        {1, 3, 9}, {3, 1, 9},
        {1, 6, 14}, {6, 1, 14},
        {3, 6, 2}, {6, 3, 2},
        {3, 4, 11}, {4, 3, 11},
        {6, 5, 9}, {5, 6, 9},
        {4, 5, 6}, {5, 4, 6}
    };
    DijkstraSolver<SimpleEdge, SimplePath,
        SimpleCmp, &graph->get_edges> solver();
    solver.find_path(1, 5);
}
