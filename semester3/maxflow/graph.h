#include <ctime>
#include <vector>
#include <list>
#include <cassert>
#include <algorithm>
#include <iostream>

typedef unsigned long size_t;
using std::vector;
using std::list;

int INF = 1000000;

struct Edge;

struct Vertex {
    int excess;
    int height;
    
    Vertex() : excess(0), height(0) {};
};

struct Edge {
    int capacity;
    int flow;
    
    Edge() : capacity(0), flow(0) {}

    int extra_capacity() {
        assert(capacity - flow >= 0);
        return (capacity - flow);
    }

    bool is_open() {
        return extra_capacity() > 0;
    }

    void run_flow(int value) {
        flow += value;
        assert(flow <= capacity);
    }
};

class Network {
    vector<Vertex> vertices;
    vector<vector<Edge> > edges;
    list<Vertex>::iterator current_vertex;

    void run_flow(size_t from, size_t to, int value) {
        edges[from][to].run_flow(value);
        edges[to][from].run_flow(-value);
        vertices[from].excess -= value;
        vertices[to].excess += value;
    }

    void push(size_t source, size_t target) {
        std::cerr << "push(" << source << ", " << target << ")" << std::endl;
        Edge& edge = edges[source][target];
        if (edge.extra_capacity() == 0 || vertices[source].height != vertices[target].height + 1) { 
            return;
        }
        assert(vertices[source].excess > 0);
        assert(vertices[source].height == vertices[target].height + 1);
        assert(edge.extra_capacity() > 0);
        int extra_flow = std::min(vertices[source].excess, edge.extra_capacity());
        run_flow(source, target, extra_flow);
    }

    void relabel(size_t vertex) {
        std::cerr << "relabel(" << vertex << ")" << std::endl;
        assert(vertices[vertex].excess > 0);
        int lowest_height = INF;
        for (size_t neighbour = 0; neighbour < vertices.size(); ++neighbour) {
            if (edges[vertex][neighbour].is_open()) {
                lowest_height = std::min(lowest_height, vertices[neighbour].height);
            }
        }
        assert(lowest_height >= vertices[vertex].height);
        vertices[vertex].height = lowest_height + 1;
    }

    bool discharge(size_t vertex) {
        std::cerr << "discharge(" << vertex << ")" << std::endl;
        bool updated = false;
        size_t current_neighbour = 0;
        while (vertices[vertex].excess > 0) {
            updated = true;
            if (current_neighbour == vertices.size()) { 
                relabel(vertex);
                current_neighbour = 0;
            }
            push(vertex, current_neighbour);
            ++current_neighbour;
        };
        return updated;
    }

    void generate_flow() {
        bool found = false;
        while (!found) {
            found = true;
            for (size_t vertex = 1; vertex < vertices.size() - 1; ++vertex) {
                if (discharge(vertex)) {
                    found = false;
                }
            }
        }
        for (size_t i = 0; i < vertices.size(); ++i) {
            for (size_t j = 0; j < vertices.size(); ++j) {
                std::cerr << edges[i][j].capacity << ' ';
            }
            std::cerr << std::endl;
        }
        for (size_t i = 0; i < vertices.size(); ++i) {
            for (size_t j = 0; j < vertices.size(); ++j) {
                std::cerr << edges[i][j].flow << ' ';
            }
            std::cerr << std::endl;
        }
    }

public:
    Network(size_t vertex_number)
        : vertices(vertex_number), edges(vertex_number, vector<Edge>(vertex_number)) {
            vertices.front().height = vertex_number;
    }

    Edge* add_edge(size_t tail, size_t head, unsigned capacity) {
        edges[tail][head].capacity = capacity;
        if (tail == 0) {
            run_flow(tail, head, capacity);
        } 
        return &edges[tail][head];
    }

    int find_flow() {
        generate_flow();
        int flow = 0;
        for (size_t vertex = 1; vertex < vertices.size(); ++vertex) {
            flow += edges[0][vertex].flow;
        }
        return flow;
    }
};

