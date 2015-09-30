#include <ctime>
#include <vector>
#include <list>
#include <cassert>
#include <algorithm>

typedef unsigned long size_t;
using std::vector;
using std::list;

unsigned INF = 1000000000;

struct Edge;

struct Vertex {
    unsigned excess;
    unsigned height;
    size_t current_neighbour;
    
    Vertex() : excess(0), height(0), current_neighbour(0) {};
};

struct Edge {
    unsigned capacity;
    int flow;
    
    Edge() : capacity(0), flow(0) {}

    unsigned extra_capacity() {
        assert(capacity - flow > 0);
        return static_cast<unsigned>(capacity - flow);
    }

    bool is_open() {
        return extra_capacity() > 0;
    }

    void run_flow(int value) {
        flow += value;
    }
};

class Graph {
    vector<Vertex> vertices;
    vector<vector<Edge>> edges;
    list<Vertex>::iterator current_vertex;

    Graph(size_t vertex_number)
        : vertices(vertex_number), edges(vertex_number, vector<Edge>(vertex_number)) {
        vertices.front().height = vertex_number;
    }

    inline bool is_out(size_t vertex) {
        return vertex == vertices.size() - 1;
    }

    void run_flow(size_t from, size_t to, int value) {
        edges[from][to].run_flow(value);
        edges[to][from].run_flow(-value);
    }

    void add_edge(size_t tail, size_t head, unsigned capacity) {
        edges[tail][head].capacity = capacity;
        if (is_out(head)) {
            edges[tail][head].run_flow(capacity);
        } 
    }

    void push(size_t source, size_t target) {
        Edge& edge = edges[source][target];
        if (edge.extra_capacity() == 0 || vertices[source].height > vertices[target].height) { 
            return;
        }
        assert(vertices[source].excess > 0);
        assert(vertices[source].height == vertices[target].height + 1);
        assert(edge.extra_capacity() > 0);
        unsigned extra_flow = std::max(vertices[source].excess, edge.extra_capacity());
        vertices[source].excess -= extra_flow;
        edge.run_flow(extra_flow);
    }

    void relabel(size_t vertex) {
        assert(vertices[vertex].excess > 0);
        unsigned lowest_height = INF;
        for (size_t neighbour = 0; neighbour < vertices.size(); ++neighbour) {
            if (edges[vertex][neighbour].is_open()) {
                lowest_height = std::min(lowest_height, vertices[neighbour].height);
            }
        }
        assert(lowest_height >= vertices[vertex].height);
        vertices[vertex].height = lowest_height + 1;
    }

    void discharge(size_t vertex) {
        while (vertices[vertex].excess > 0) {
            if (vertices[vertex].current_neighbour == vertices.size()) { 
                relabel(vertex);
                vertices[vertex].current_neighbour = 0;
            }
            push(vertex, vertices[vertex].current_neighbour);
            ++vertices[vertex].current_neighbour;
        };
    }

};

