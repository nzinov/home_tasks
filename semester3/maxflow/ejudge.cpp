#include <vector>
#include <algorithm>
#include <queue>

typedef unsigned long size_t;
using std::vector;
using std::queue;

long long INF = 1000000;

struct Edge;

struct Vertex {
    long long excess;
    long long height;
    size_t current;
    vector<size_t> neighbours;
    
    Vertex() : excess(0), height(0), current(0) {};
};

struct Edge {
    long long capacity;
    long long flow;
    
    Edge() : capacity(0), flow(0) {}

    inline long long extra_capacity() {
        return (capacity - flow);
    }

    inline bool is_open() {
        return capacity > flow;
    }
};

struct Network {
    vector<Vertex> vertices;
    vector<vector<Edge> > edges;
    queue<size_t> q;

    inline void set_height(size_t vertex, long long height) {
        vertices[vertex].height = height;
    }

    inline void run_flow(size_t from, size_t to, long long value) {
        edges[from][to].flow += value;
        edges[to][from].flow -= value;
        vertices[from].excess -= value;
        bool flag = vertices[to].excess > 0;
        vertices[to].excess += value;
        if (!flag && to != 0 && to != vertices.size() - 1 && vertices[to].excess > 0) { 
            q.push(to);
        }
    }

    inline void push(size_t source, size_t target) {
        Edge& edge = edges[source][target];
        if (!edge.is_open() || vertices[source].height != vertices[target].height + 1) { 
            return;
        }
        long long extra_flow = std::min(vertices[source].excess, edge.extra_capacity());
        run_flow(source, target, extra_flow);
    }

    inline void relabel(size_t vertex) {
        long long lowest_height = INF;
        for (size_t i = 0; i < vertices[vertex].neighbours.size(); ++i) {
            if (edges[vertex][vertices[vertex].neighbours[i]].is_open()) {
                lowest_height = std::min(lowest_height, vertices[vertices[vertex].neighbours[i]].height);
            }
        }
        set_height(vertex, lowest_height + 1);
    }

    inline bool discharge(size_t vertex) {
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

    void generate_flow() {
        while (!q.empty()) {
            discharge(q.front());
            q.pop();
        }
    }

public:
    Network(size_t vertex_number)
        : vertices(vertex_number), edges(vertex_number, vector<Edge>(vertex_number)) {
            vertices.front().height = vertex_number;
    }

    Edge* add_edge(size_t tail, size_t head, unsigned capacity) {
        edges[tail][head].capacity += capacity;
        vertices[tail].neighbours.push_back(head);
        vertices[head].neighbours.push_back(tail);
        if (tail == 0) {
            run_flow(tail, head, capacity);
        } 
        return &edges[tail][head];
    }

    long long find_flow() {
        generate_flow();
        long long flow = 0;
        for (size_t vertex = 1; vertex < vertices.size(); ++vertex) {
            flow += edges[0][vertex].flow;
        }
        return flow;
    }
};

struct EdgePart {
    Edge* edge;
    long long capacity;

    EdgePart(Edge* edge, long long capacity) : edge(edge), capacity(capacity) {}
    
    long long flow() {
        long long flow = std::max(0LL, std::min(capacity, edge->flow));
        edge->flow -= flow;
        return flow;
    }
};

int main() {
    size_t vertex_count, edge_count;
    scanf("%lu %lu", &vertex_count, &edge_count);
    Network graph(vertex_count);
    vector<EdgePart> edges;
    edges.reserve(edge_count);
    for (size_t i = 0; i < edge_count; ++i) {
        size_t tail, head, capacity;
        scanf("%lu %lu %lu", &tail, &head, &capacity);
        edges.push_back(EdgePart(graph.add_edge(tail - 1, head - 1, capacity), capacity));
    }
    printf("%llu\n", graph.find_flow());
    for (size_t i = 0; i < edges.size(); ++i) {
        printf("%llu\n", edges[i].flow());
    }
    for (Vertex v : graph.vertices) {
        printf("%llu ", v.height);
    }
}
