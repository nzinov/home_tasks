#include "aho.h"

using std::string;

Node::Node() : suffix(nullptr), terminal(false) {
    for (short i = 0; i < ALPABET_LENGTH; ++i) {
        children[i] = 0;
    }
}

inline short Node::index(char ch) {
    return static_cast<short>(ch - 'a');
}

Node* Node::go(short mark) {
    if (children[mark]) {
        return children[mark];
    }
    if (!suffix) {
        return this;
    }
    return suffix->go(mark);
}

Node* Node::go(char mark) {
    return go(index(mark));
}

Node* Node::force_go(char mark) {
    if (!children[index(mark)]) {
        children[index(mark)] = new Node();
    }
    return children[index(mark)];
}

Node::~Node() {
    for (short i = 0; i < ALPABET_LENGTH; ++i) {
        delete children[i];
    }
}

void Node::calculate_links(Node* link) {
    if (link) {
        suffix = link;
    }
    for (short i = 0; i < ALPABET_LENGTH; ++i) {
        if (children[i]) {
            children[i]->calculate_links(suffix ? suffix->go(i) : this); 
        }
    }
}

void Trie::add_string(const string& str, int begin, int end, short id) {
    Node* cursor = &root_node;
    for (int i = begin; i < end; ++i) {
        cursor = cursor->force_go(str[i]);
    }
    cursor->terminal = true;
    cursor->substring_id.push_back(id);
};

void Trie::finalize() {
    root_node.calculate_links();
}

void Trie::process(const string& text, std::function<void(int position, int substring_id)> callback) {
    Node* current_state = &root_node;
    for (int i = 0; i < text.length(); ++i) {
        current_state = current_state->go(text[i]);
        while (true) {
            if (current_state->terminal) {
                for (auto cursor = current_state->substring_id.begin(); cursor != current_state->substring_id.end(); ++cursor) {
                    callback(i, *cursor);
                }
            }
            if (!current_state->suffix) {
                break;
            }
            current_state = current_state->suffix;
        }
    }
}
