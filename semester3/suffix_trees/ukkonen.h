#include <string>

int INF = 1000000000;

struct Node;

struct Edge {
    int left;
    int right;
    Node* from;
    Node* to;

    Edge(Node* from) : left(0), right(0), from(from), to(nullptr) {
    }

    ~Edge() {
        delete to;
    };

    bool empty() {
        return to == nullptr;
    }
};

const int ALPHABET_SIZE = 26;

struct Node {
    Node* suffix_link;
    Edge transitions[ALPHABET_SIZE];

    Node() {
        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            transitions[i] = Edge(this);
        }
    }

    ~Node() {
    }
};

struct Pointer {
    Edge* edge;
    int offset;
    const std::string& data;

    Pointer(Edge* edge, int offset, const std::string& data) : edge(edge), offset(offset), data(data) {
    }

    Pointer make_pointer(Edge* edge, int offset) {
        return Pointer(edge, offset, data);
    }

    Pointer materialize() {
        if (offset == 0) {
            return *this;
        }
        Node* new_node = new Node();
        short new_char = data[edge->left + offset - 1] - 'a';
        Edge& new_edge = new_node->transitions[new_char];
        new_edge.to = edge->to;
        new_edge.left = edge->left + offset;
        new_edge.right = edge->right;
        new_edge.from = new_node;
        edge->right = edge->left + offset;
        edge->to = new_node;
        return make_pointer(&new_edge, 0);
    }

    bool can_go(short ch) {
        return !node->transitions[ch].empty();
    }

    Pointer get_suffix() {
        Node* current = edge->from->suffix_link;
        if (current == nullptr) {
            return *this;
        }
        int current_offset = 0;
        while (true) {
            Edge* current_edge = current->transitions[data[edge->left + current_offset] - 'a'];
            if (current_offset  + edge->right - edge->left >= offset) {
                return make_pointer(current_edge, offset - current_offset);
            }
            current_offset += edge->right - edge->left;
            current = current_edge->to;
        }
    };

};

class SuffixTree {
    Node root;
    Node joker;
    std::string data;

    SuffixTree() {
        joker.suffix_link = nullptr;
        root.suffix_link = &joker;
        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            joker.transitions[i].to = &root;
        }
    }

    void build() {
        Pointer last_processed = Pointer(&joker, 0, 0);
        for (int next_symbol = 0; next_symbol < data.length(); ++next_symbol) {                     
            short ch = data[next_symbol] - 'a';
            while (true) {
                if (!last_processed.can_go(ch)) {
                    Edge& edge = last_processed.node->transitions[ch];
                    edge.to = new Node();
                    edge.left = next_symbol;
                    edge.right = INF;
                    last_processed = last_processed.get_suffix();
                } else {
                }
            }
        }
    }
};
    }
};
