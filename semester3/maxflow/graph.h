#include <vector>
#include <list>
#include <cassert>
#include <algorithm>

using std::vector;
using std::list;

struct Edge;

struct Vertex {
    unsigned excess;
    unsigned height;
    vector<Edge*> adjacent_edges;
    vector<Edge*>::iterator current_edge;
};

struct Edge {
    Vertex* tail;
    Vertex* head;
    unsigned capacity;
    int flow;
    
    Edge(Vertex* tail, Vertex* head, unsigned capacity) : tail(tail), head(head), capacity(capacity), flow(0) {}

    unsigned extra_capacity() {
        assert(capacity - flow > 0);
        return static_cast<unsigned>(capacity - flow);
    }

    bool is_open() {
        return extra_capacity() > 0;
    }
};

class Graph {
    list<Vertex> vertices;
    vector<Edge> edges;
    list<Vertex>::iterator current_vertex;

    void add_edge(Vertex* tail, Vertex* head, unsigned capacity) {
        edges.emplace_back(tail, head, capacity);
        tail->adjacent_edges.push_back(&edges);
    }

    void add_vertex() {
        vertices.emplace_back(vertices.size());
    }

    void push(Vertex* source, Vertex* target) {
        Edge& edge = get_edge(source, target);
        assert(source->excess > 0);
        assert(source->height == target->height + 1);
        assert(edge.extra_capacity() > 0);
        unsigned extra_flow = std::max(source->excess, edge.extra_capacity());
        source->excess -= extra_flow;
        edge.flow += extra_flow;
    }

    void relabel(Vertex* vertex) {
        assert(vertex->excess > 0);
        unsigned lowest_height = INF;
        for (Vertex& adjacent : vertices) {
            if (!get_edge(vertex, &adjacent).is_open()) {
                continue;
            }
            lowest_height = std::min(lowest_height, adjacent.height);
        }
        assert(lowest_height >= vertex->height);
        vertex->height = lowest_height + 1;
    }

    void discharge(Vertex* vertex) {
        while (vertex->excess > 0) {
            if (current_vertex == vertices.end()) { 
            }
        };
    }

};

