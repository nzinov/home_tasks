#include <ctime>
#include <vector>
#include <cassert>
#include <algorithm>
#include <queue>
#include "graph.h"
#include "log.h"

using std::vector;
using std::queue;

Vertex::Vertex() : excess(0), height(0), current(0) {};

Edge::Edge() : capacity(0), flow(0) {}

inline long long Edge::extra_capacity() {
    assert(capacity - flow >= 0);
    return (capacity - flow);
}

inline bool Edge::is_open() {
    return capacity > flow;
}

inline void Network::set_height(size_t vertex, long long height) {
    long long prev_height = vertices[vertex].height;
    vertices[vertex].height = height;
    heights[prev_height]--;
    heights[height]++;
    if (heights[prev_height] == 0) {
        log->add(Action(Gap{prev_height}), *this);
        for (size_t v = 0; v < vertices.size(); ++v) {
            if (vertices[v].height > prev_height) {
                vertices[v].height = vertices.size();
            }
        }
    }
}

inline void Network::run_flow(size_t from, size_t to, long long value) {
    edges[from][to].flow += value;
    edges[to][from].flow -= value;
    vertices[from].excess -= value;
    bool flag = vertices[to].excess > 0;
    vertices[to].excess += value;
    if (!flag && to != 0 && to != vertices.size() - 1 && vertices[to].excess > 0) { 
        q.push(to);
    }
}

inline void Network::push(size_t source, size_t target) {
    Edge& edge = edges[source][target];
    if (!edge.is_open() || vertices[source].height != vertices[target].height + 1) { 
        return;
    }
    assert(vertices[source].excess > 0);
    assert(vertices[source].height == vertices[target].height + 1);
    assert(edge.extra_capacity() > 0);
    log->add(Action(Push{source, target}), *this);
    long long extra_flow = std::min(vertices[source].excess, edge.extra_capacity());
    run_flow(source, target, extra_flow);
}

inline void Network::relabel(size_t vertex) {
    log->add(Action(Relabel{vertex}), *this);
    long long lowest_height = INF;
    for (size_t i = 0; i < vertices[vertex].neighbours.size(); ++i) {
        if (edges[vertex][vertices[vertex].neighbours[i]].is_open()) {
            lowest_height = std::min(lowest_height, vertices[vertices[vertex].neighbours[i]].height);
        }
    }
    set_height(vertex, lowest_height + 1);
}

inline bool Network::discharge(size_t vertex) {
    bool updated = false;
    while (vertices[vertex].excess > 0) {
        updated = true;
        if (vertices[vertex].current == vertices.size()) { 
            relabel(vertex);
            vertices[vertex].current = 0;
        }
        push(vertex, vertices[vertex].current);
        vertices[vertex].current++;
    };
    return updated;
}

void Network::generate_flow() {
    while (!q.empty()) {
        discharge(q.front());
        q.pop();
    }
}

Network::Network(size_t vertex_number, Log *log) :
    vertices(vertex_number),
    edges(vertex_number, vector<Edge>(vertex_number)),
    heights(vertex_number*2 + 1),
    log(log) {
        if (vertex_number == 0) {
            return;
        }
        vertices.front().height = vertex_number;
        heights[0] = vertex_number - 1;
        heights[vertex_number] = 1;
}

Edge* Network::add_edge(size_t tail, size_t head, unsigned capacity) {
    edges[tail][head].capacity += capacity;
    vertices[tail].neighbours.push_back(head);
    vertices[head].neighbours.push_back(tail);
    if (tail == 0) {
        run_flow(tail, head, capacity);
    } 
    return &edges[tail][head];
}

long long Network::find_flow() {
    generate_flow();
    long long flow = 0;
    for (size_t vertex = 1; vertex < vertices.size(); ++vertex) {
        flow += edges[0][vertex].flow;
    }
    return flow;
}

const Vertex* Network::get_vertex(size_t id) const {
    return &vertices[id];
}

const Edge* Network::get_edge(size_t tail, size_t head) const {
    return &edges[tail][head];
}
