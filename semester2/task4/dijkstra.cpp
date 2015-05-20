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

/*
 * cmp must return true if and only if
 * the first path is shorter than the second
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
        path.push_back(end);
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

    vector<size_t> find_path(size_t a, size_t b) {
        vertices.clear();
        queue.insert(a); 
        start = a;
        end = b;
        allocate(b);
        while (vertices[b].state != FREEZED) {
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
        return get_path();
    }
};

template<typename Edge, typename Path,
        typename _Get_edges>
        DijkstraSolver<Edge, Path, _Get_edges>
        get_solver(_Get_edges get_edges) {
    return DijkstraSolver<Edge, Path, _Get_edges>(get_edges);
}
#endif
