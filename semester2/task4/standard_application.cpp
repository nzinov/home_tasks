#include "../googletest/include/gtest/gtest.h"
#include <functional>
#include "dijkstra.cpp"

using namespace std::placeholders;

typedef long long length_t;

struct SimpleEdge : GenericEdge {
    length_t weight;

    SimpleEdge(size_t tail, size_t head, size_t weight) :
        GenericEdge(tail, head), weight(weight) {}
};

struct SimplePath {
    length_t length;
    SimplePath(int length = 0) : length(length) {}

    SimplePath extend(const SimpleEdge& edge) {
        return SimplePath(length + edge.weight);
    }
};

bool SimpleCmp(SimplePath& a, SimplePath& b) {
    return a.length < b.length;
}

vector<SimpleEdge> get_edges(vector<SimpleEdge> edges, size_t v) {
    vector<SimpleEdge> current;
    for (SimpleEdge edge : edges) {
        if (edge.tail == v) {
            current.push_back(edge);
        }
    }
    return current;
}

TEST(DijkstraTest, Simple) {
    vector<SimpleEdge> edges = {
        SimpleEdge(1, 2, 7),
        SimpleEdge(1, 3, 9),
        SimpleEdge(1, 6, 14),
        SimpleEdge(2, 3, 10),
        SimpleEdge(2, 4, 15),
        SimpleEdge(3, 6, 2),
        SimpleEdge(3, 4, 11),
        SimpleEdge(6, 5, 9),
        SimpleEdge(4, 5, 6),
    };
    for (SimpleEdge edge : edges) {
        edges.push_back(SimpleEdge(edge.head, edge.tail, edge.weight));
    }
    DijkstraSolver<SimpleEdge, SimplePath,
        bool(*)(SimplePath&, SimplePath&),
        std::function<vector<SimpleEdge>(size_t)> >
            solver(SimpleCmp,
                    std::bind(get_edges, edges, _1));
    solver.find_path(1, 5);
}
