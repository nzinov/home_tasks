#ifndef GRAPH_MAXFLOW
#define GRAPH_MAXFLOW
#include <ctime>
#include <vector>
#include <queue>

typedef unsigned long size_t;

const long long INF = 1e9;

struct Edge;

struct Vertex {
    long long excess;
    long long height;
    size_t current;
    std::vector<size_t> neighbours;
    Vertex();
};

struct Edge {
    long long capacity;
    long long flow;
    
    Edge();

    inline long long extra_capacity();
    inline bool is_open();
};

class Network {
    std::vector<Vertex> vertices;
    std::vector<std::vector<Edge> > edges;
    std::queue<size_t> q;
    std::vector<size_t> heights;

    inline void set_height(size_t vertex, long long height);
    inline void run_flow(size_t from, size_t to, long long value);
    inline void push(size_t source, size_t target);
    inline void relabel(size_t vertex);
    inline bool discharge(size_t vertex);
    void generate_flow();

public:
    Network(size_t vertex_number);
    Edge* add_edge(size_t tail, size_t head, unsigned capacity);
    long long find_flow();
};
#endif
