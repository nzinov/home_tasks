#include <vector>
#include <cstdlib>
#include <set>
#include <functional>

using std::vector;
using std::set;

typedef unsigned int size_t;

struct SimpleVertex {
};

enum State {
    NOT_VISITED,
    VISITED,
    FREEZED
};

template<typename Vertex, typename Path>struct VertexStruct : public Vertex {
    Path bestpath;
    size_t parent;
    State state;

    VertexStruct(Vertex vertex) : Vertex(vertex) {
        state = NOT_VISITED;
    }
};

template<typename Edge> struct EdgeStruct : public Edge {
    size_t start;
    size_t end;

    EdgeStruct(size_t start, size_t end): start(start), end(end) {}
};

struct SimpleEdge {
    int weight;
    size_t end;
    SimpleEdge(int weight, size_t end) : weight(weight), end(end) {}
};

struct SimplePath {
    int length;
    SimplePath(int length) : length(length) {}

    SimplePath extend(SimpleEdge& edge) {
        return SimplePath(length + edge.weight);
    }
};

bool SimpleCmp(SimplePath& a, SimplePath& b) {
    return a.length < b.length;
}

template<typename Vertex, typename Edge, typename Path, typename cmp> class Graph {
    vector<VertexStruct<Vertex, Path> > vertices;
    vector<vector<EdgeStruct<Edge> > > edges;

    set<size_t, std::mem_fun_t<bool, Graph> > queue;

    Graph() {
        queue.cmp = std::mem_fn(&this->vertex_cmp);
    }

    bool vertex_cmp(size_t a, size_t b) {
        return cmp(vertices[a].bestpath, vertices[b].bestpath);
    }
    
    vector<size_t> find_path(size_t a, size_t b) {
        queue.insert(a); 
        while (vertices[b].state != FREEZED) {
            size_t cur = queue.front();
            queue.remove(cur);
            vertices[cur].state = FREEZED;
            for (Edge& edge : edges[cur]) {
                relax(edge);
            }
        }
        return path_iterator(b);
    }

    void relax(EdgeStruct<Edge> edge) {
        if (vertices[edge.end].state == FREEZED) {
            return;
        }
        Path new_path = vertices[edge.start].best_path.extend(edge);
        if (vertices[edge.end].state == NOT_VISITED) {
            vertices[edge.end].best_path = new_path;
            queue.insert(edge.end);
        } else {
            if (cmp(new_path, vertices[edge.end].best_path)) {
                queue.remove(edge.end);
                vertices[edge.end].state = VISITED;
                vertices[edge.end].path = new_path;
                vertices[edge.end].parent = edge.start;
            }
        }
    }

    struct path_iterator : std::iterator<std::output_iterator_tag, size_t, void, size_t&, size_t*> {
        Graph parent;
        size_t vertex;

        path_iterator(Graph parent, size_t vertex) : parent(parent), vertex(vertex) {}
        size_t operator*() {
            return vertex;
        }

        bool operator==(path_iterator& other) {
            return other.vertex == vertex;
        }

        size_t operator++() {
            vertex = parent->vertices[vertex].parent;
        }
    };
};
