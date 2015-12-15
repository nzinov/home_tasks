#include <string>
#include <cassert>
#include <iostream>

int INF = 1000000000;

struct Node;

struct Edge {
    int left;
    int right;
    Node* from;
    Node* to;

    Edge(Node* from = NULL) : left(0), right(0), from(from), to(NULL) {
    }

    ~Edge();

    bool empty() {
        return to == NULL;
    }

    int length() {
        return right - left;
    }
};

const int ALPHABET_SIZE = 26;

struct Node {
    bool is_root;
    Node* suffix_link;
    Edge transitions[ALPHABET_SIZE];

    Node(bool is_root = false) : is_root(is_root) {
        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            transitions[i] = Edge(this);
        }
    }

    ~Node() {
    }

    bool can_go(short ch) {
        return !transitions[ch].empty();
    }
};

Edge::~Edge() {
    if (to != NULL && !to->is_root) {
        delete to;
    }
};

struct Pointer {
    /*
     * Represents a position in tree
     * edge - edge on which the position lies
     * offset - number of symbols passed by that edge
     * data - reference to the data string for lookup
     */
    Edge* edge;
    int offset;
    const std::string* data;

    Pointer(Edge* edge = NULL, int offset = 0, const std::string* data = NULL) : edge(edge), offset(offset), data(data) {
    }

    Pointer make_pointer(Edge* edge, int offset) {
        return Pointer(edge, offset, data);
    }

    Pointer materialize() {
        if (offset == edge->length()) {
            return *this;
        }
        Node* new_node = new Node();
        short new_char = symbol_at(offset);
        Edge* new_edge = &new_node->transitions[new_char];
        new_edge->to = edge->to;
        new_edge->left = edge->left + offset;
        new_edge->right = edge->right;
        new_edge->from = new_node;
        new_node->suffix_link = get_suffix().materialize().to();
        edge->right = edge->left + offset;
        edge->to = new_node;
        return make_pointer(edge, edge->length());
    }

    Pointer get_suffix() {
        Node* current = edge->from->suffix_link;
        if (current == NULL) {
            return *this;
        }
        int current_offset = 0;
        while (true) {
            Edge* current_edge = &current->transitions[symbol_at(current_offset)];
            if (current_offset  + current_edge->length() >= offset) {
                return make_pointer(current_edge, offset - current_offset);
            }
            current_offset += current_edge->length();
            current = current_edge->to;
        }
    };

    Node* from() {
        return edge->from;
    }

    Node* to() {
        return edge->to;
    }

    short symbol_at(int index) {
        return (*data)[edge->left + index] - 'a';
    }

    short next_symbol() {
        return symbol_at(offset);
    }
};

class SuffixTree {
    Node root;
    Node joker;
    std::string data;

public:
    SuffixTree(std::string str) : root(true), data(str) {
        joker.suffix_link = NULL;
        root.suffix_link = &joker;
        //data.push_back('$');
        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            joker.transitions[i].to = &root;
            joker.transitions[i].right = 1;
        }
    }

    void build() {
        Pointer last_processed = make_pointer(&joker.transitions[0], 1);
        for (int i = 0; i < data.length(); ++i) {                     
            short next_symbol = data[i] - 'a';
            while (true) {
                if (last_processed.offset == last_processed.edge->length()) { //Pointer is on a real node
                    if (!last_processed.to()->can_go(next_symbol)) {
                        Edge& edge = last_processed.to()->transitions[next_symbol];
                        edge.to = new Node();
                        edge.left = i;
                        edge.right = INF;
                        if (last_processed.from() == &joker) {
                            break;
                        }
                        last_processed = last_processed.get_suffix();
                    } else {
                        last_processed = make_pointer(&last_processed.to()->transitions[next_symbol], 1);
                        break;
                    }
                } else {
                    if (last_processed.next_symbol() == next_symbol) {
                        last_processed.offset++;
                        break;
                    } else {
                        last_processed = last_processed.materialize();
                    }
                }
            }
        }
    }

    std::string gcs(std::string other) {
        int depth = 0;
        int max_depth = 0;
        int right = 0;
        Pointer current_position = make_pointer(&joker.transitions[0], 1);
        for (int i = 0; i < other.length(); ++i) {
            short current_symbol = other[i] - 'a';
            while (true) {
                if (current_position.offset == current_position.edge->length()) {
                    if (current_position.to()->can_go(current_symbol)) {
                        ++depth;
                        current_position = make_pointer(&current_position.to()->transitions[current_symbol], 1);
                        break;
                    }
                    if (current_position.from() == &joker) {
                        --depth;
                        break;
                    }
                    current_position = current_position.get_suffix();
                    --depth;
                } else {
                    if ((current_position.edge->left + current_position.offset >= static_cast<int>(data.length()))
                            || (current_position.next_symbol() != current_symbol)) {
                            current_position = current_position.get_suffix();
                            --depth;
                    } else {
                        ++depth;
                        ++current_position.offset;
                        break;
                    }
                }
            }
            if (depth > max_depth) {
                max_depth = depth;
                right = current_position.edge->left + current_position.offset;
            }
        }
        return data.substr(right - max_depth, max_depth);
    }

    Pointer make_pointer(Edge* edge, int offset) {
        return Pointer(edge, offset, &data);
    }
};

int main() {
    std::string a, b;
    std::cin >> a >> b;
    SuffixTree tree(a);
    tree.build();
    std::cout << tree.gcs(b);
}
    
