#include "../googletest/include/gtest/gtest.h"
#include <functional>
#include "dijkstra.cpp"

using namespace std::placeholders;

typedef long long length_t;

struct SimpleEdge : GenericEdge {
    length_t weight;

    SimpleEdge(size_t tail, size_t head, length_t weight) :
        GenericEdge(tail, head), weight(weight) {}
};

struct SimplePath {
    length_t length;
    SimplePath(length_t length = 0) : length(length) {}

    SimplePath extend(const SimpleEdge& edge) {
        return SimplePath(length + edge.weight);
    }
};

bool SimpleCmp(SimplePath& a, SimplePath& b) {
    return a.length < b.length;
}

struct SimpleGraph {
    vector<vector<SimpleEdge> > edges;

    SimpleGraph(size_t vertex_number) : edges(vertex_number, vector<SimpleEdge>(vertex_number)) {}

    const vector<SimpleEdge>& get_edges(size_t v) {
        return edges[v];
    }

    void add_edge(size_t a, size_t b, size_t weight) {
        edges[a].push_back(SimpleEdge(a, b, weight));
        edges[b].push_back(SimpleEdge(b, a, weight));
    };
};

TEST(DijkstraTest, Simple) {
    SimpleGraph graph;
    graph.add_edge(1, 2, 7);
    graph.add_edge(1, 3, 9);
    graph.add_edge(1, 6, 14);
    graph.add_edge(2, 3, 10);
    graph.add_edge(2, 4, 15);
    graph.add_edge(3, 6, 2);
    graph.add_edge(3, 4, 11);
    graph.add_edge(6, 5, 9);
    graph.add_edge(4, 5, 6);
    auto solver = get_solver<SimpleEdge, SimplePath>(
        std::bind(SimpleCmp, _1, _2),
        std::bind(&SimpleGraph::get_edges, graph, _1));
    solver.find_path(1, 5);
}
