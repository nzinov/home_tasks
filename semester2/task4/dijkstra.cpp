#ifndef dijkstra
#define dijkstra

#include <vector>
#include <cstdlib>
#include <set>
#include <functional>
#include <algorithm>

using std::vector;
using std::set;
using namespace std::placeholders;

struct SimpleVertex {
};

enum State {
    NOT_VISITED,
    VISITED,
    FREEZED
};

template<typename Path>struct VertexStruct {
    Path best_path;
    size_t precursor;
    State state;

    VertexStruct() {
        state = NOT_VISITED;
    }
};

struct GenericEdge {
    size_t tail;
    size_t head;

    GenericEdge(size_t tail, size_t head) : tail(tail), head(head) {}
};

struct NoPathException {
};

/*
 * Customized version of Dijkstra algorithm
 *
 * 'Edge' template argument must be a struct
 * representing edge of your graph
 * with members 'tail' ans 'head' of size_t and any other members.
 * You are encouraged to inherit from GenericEdge struct.
 *
 * 'Path' template argument must be a struct
 * representing path in your graph
 * with operator< overloaded in terms of shorter path
 * and method 'extend' taking Edge and returning
 * a new path resulting from extending current path by the edge.
 * 
 * 'get_edges' parameter is a function
 * with size_t argument v
 * returning container of all edges begining in v.
 */
template<typename Edge, typename Path,
        typename _Get_edges> class DijkstraSolver {
    vector<VertexStruct<Path> > vertices;
    set<size_t, std::function<bool(size_t, size_t)>> queue;
    size_t start, end;
    _Get_edges get_edges;

    bool vertex_cmp(size_t a, size_t b) {
        if (!(vertices[a].best_path < vertices[b].best_path ||
                vertices[b].best_path < vertices[a].best_path)) {
            return a < b;
        }
        return vertices[a].best_path < vertices[b].best_path;
    }
    
    void allocate(size_t v) {
        if (vertices.size() <= v) {
            vertices.resize(v+1);
        }
    }

    vector<size_t> get_path() {
        vector<size_t> path;
        for (size_t cursor = end; cursor != start;
                cursor = vertices[cursor].precursor) {
            path.push_back(cursor);
        }
        path.push_back(start);
        std::reverse(path.begin(), path.end());
        return path;
    }


    void relax(const Edge& edge) {
        VertexStruct<Path>& target = vertices[edge.head];
        if (target.state == FREEZED) {
            return;
        }
        allocate(edge.head);
        Path new_path = vertices[edge.tail].best_path.extend(edge);
        if (target.state == NOT_VISITED ||
                new_path < vertices[edge.head].best_path) {
            queue.erase(edge.head);
            target.state = VISITED;
            target.best_path = new_path;
            target.precursor = edge.tail;
            queue.insert(edge.head);
        }
    }

public:
    DijkstraSolver(_Get_edges get_edges) :
        get_edges(get_edges),
        queue(std::bind(&DijkstraSolver::vertex_cmp, this, _1, _2)) {}

    struct Answer {
        Path best_path;
        vector<size_t> vertices;

        Answer(Path path, vector<size_t> vertices) : best_path(path), vertices(vertices) {}
    };
    /*
     * arguments: vertices to find path between
     * returns: pair of the best Path between a and b and vector<size_t> of vertices on that path
     * throws: NoPathException if there is no path between a and b
     */
    Answer find_path(size_t a, size_t b) {
        vertices.clear();
        queue.insert(a); 
        start = a;
        end = b;
        allocate(b);
        while (vertices[b].state != FREEZED && !queue.empty()) {
            if (queue.empty()) {
            }
            size_t cur = *queue.begin();
            allocate(cur);
            queue.erase(queue.begin());
            vertices[cur].state = FREEZED;
            for (const Edge& edge : get_edges(cur)) {
                relax(edge);
            }
        }
        if (vertices[b].state == NOT_VISITED) {
            throw NoPathException();
        }
        return Answer(vertices[b].best_path, get_path());
    }
};

/*
   Returns a new instance of DijkstraSolver
 */
template<typename Edge, typename Path,
        typename _Get_edges>
        DijkstraSolver<Edge, Path, _Get_edges>
        get_solver(_Get_edges get_edges) {
    return DijkstraSolver<Edge, Path, _Get_edges>(get_edges);
}
#endif
