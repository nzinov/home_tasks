#ifndef dijkstra
#define dijkstra

#include <vector>
#include <cstdlib>
#include <set>
#include <functional>
#include <algorithm>

using std::vector;
using std::set;

struct SimpleVertex {
};

enum State {
    NOT_VISITED,
    VISITED,
    FREEZED
};

template<typename Path>struct VertexStruct {
    Path bestpath;
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
    typename cmp, typename get_edges> class DijkstraSolver {
    vector<VertexStruct<Path> > vertices;
    set<size_t, std::function<bool(size_t, size_t)> > queue;
    size_t start, end;

    DijkstraSolver() :
        queue(std::function<bool(size_t, size_t)>(&this->vertex_cmp)) {}

    bool vertex_cmp(size_t a, size_t b) {
        return cmp(vertices[a].bestpath, vertices[b].bestpath);
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
                cmp(new_path, vertices[edge.end].best_path)) {
            queue.erase(edge.head);
            target.state = VISITED;
            target.path = new_path;
            target.precursor = edge.start;
        }
    }

public:
    vector<size_t> find_path(size_t a, size_t b) {
        vertices.clear()
        queue.insert(a); 
        start = a;
        end = b;
        while (vertices[b].state != FREEZED) {
            size_t cur = *queue.begin();
            allocate(cur);
            queue.erase(queue.begin());
            vertices[cur].state = FREEZED;
            for (Edge& edge : get_edges(cur)) {
                relax(edge);
            }
        }
        return get_path();
    }
};
#endif
