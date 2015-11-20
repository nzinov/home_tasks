#include "aho.h"

using std::string;

Node::Node(Node* parent, char mark) : parent(parent), mark(mark), suffix(NULL), terminal_suffix(NULL), terminal(false) {
    for (short i = 0; i < ALPABET_LENGTH; ++i) {
        children[i] = NULL;
        transition[i] = NULL;
    }
}

inline short Node::index(char ch) {
    return static_cast<short>(ch - 'a');
}

Node* Node::go(short mark) {
    if (!transition[mark]) {
        if (children[mark]) {
            return children[mark];
        }
        if (!parent) {
            return this;
        }
        transition[mark] = get_suffix()->go(mark);
    }
    return transition[mark];
}

Node* Node::go(char mark) {
    return go(index(mark));
}

Node* Node::force_go(char mark) {
    if (!children[index(mark)]) {
        children[index(mark)] = new Node(this, mark);
    }
    return children[index(mark)];
}

Node::~Node() {
    for (short i = 0; i < ALPABET_LENGTH; ++i) {
        delete children[i];
    }
}

Node* Node::get_suffix() {
    if (!parent) {
        return this;
    }
    if (!parent->parent) {
        return parent;
    }
    if (!suffix) {
        suffix = parent->get_suffix()->go(mark);
    }
    return suffix;
}

Node* Node::get_terminal_suffix() {
    if (!parent) {
        return NULL;
    }
    if (!terminal_suffix) {
        if (get_suffix()->terminal) {
            terminal_suffix = get_suffix();
        } else {
            terminal_suffix = get_suffix()->get_terminal_suffix();
        }
    }
    return terminal_suffix;
}

void Trie::add_string(const string& str, int begin, int end, short id) {
    Node* cursor = &root_node;
    for (int i = begin; i < end; ++i) {
        cursor = cursor->force_go(str[i]);
    }
    cursor->terminal = true;
    cursor->substring_id.push_back(id);
};

void Trie::process(const string& text, std::function<void(int position, int substring_id)> callback) {
    Node* current_state = &root_node;
    for (int i = 0; i < text.length(); ++i) {
        current_state = current_state->go(text[i]);
        Node* cursor = current_state;
        while (true) {
            if (cursor->terminal) {
                for (auto el = cursor->substring_id.begin(); el != cursor->substring_id.end(); ++el) {
                    callback(i, *el);
                }
            }
            if (!cursor->get_terminal_suffix()) {
                break;
            }
            cursor = cursor->get_terminal_suffix();
        }
    }
}
