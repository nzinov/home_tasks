#include <vector>
#include <string>
#include <functional>
#include <iostream>

static const int ALPABET_LENGTH = 26;

struct Node {
    Node* parent;
    char mark;
    Node* suffix;
    Node* terminal_suffix;
    Node* children[ALPABET_LENGTH];
    Node* transition[ALPABET_LENGTH];
    bool terminal;
    std::vector<short> substring_id;

    Node(Node* parent = NULL, char mark = '\0');
    ~Node();
    inline short index(char ch);
    Node* go(short mark);
    Node* go(char mark);
    Node* force_go(char mark);
    Node* get_suffix();
    Node* get_terminal_suffix();
};

class Trie {
    Node root_node;

public:
    Trie() {}
    ~Trie() {}
    void add_string(const std::string& str, int begin, int end, short id);
    void process(const std::string& text, std::function<void(int position, int substring_id)> callback);
};

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
class Matcher {
    const std::string& pattern;
    Trie trie;
    std::vector<int> offsets;
    std::vector<int> counts;

public:
    Matcher(const std::string& pattern);
    ~Matcher() {};
    void process_occurence(int position, int block_id) {
        if (position - offsets[block_id] + 1>= 0) {
            counts[position - offsets[block_id] + 1]++;
        }
    }

    void find_matches(const std::string& text, void (*callback)(int position));
};

Matcher::Matcher(const std::string& pattern) : pattern(pattern), counts(pattern.length()) {
    int position = 0;
    for (int i = 0; i < pattern.length(); ++i) {
        if (pattern[i] == '?') {
            if (i > position) {
                offsets.push_back(i);
                trie.add_string(pattern, position, i, offsets.size() - 1);
            }
            position = i + 1;
        }
    }
    if (position < pattern.length()) {
        offsets.push_back(pattern.length());
        trie.add_string(pattern, position, pattern.length(), offsets.size() - 1);
    }
}

void Matcher::find_matches(const std::string& text, void (*callback)(int position)) {
    if (pattern.length() == 0) {
        return;
    }
    counts.resize(text.length());
    using namespace std::placeholders;
    trie.process(text, std::bind(&Matcher::process_occurence, this, _1, _2)); 
    if (text.length() >= pattern.length()) {
        for (int i = 0; i < text.length() - pattern.length() + 1; ++i) {
           if (counts[i] == offsets.size()) {
               callback(i);
           }
        }
    }
}

void print_answer(int position) {
    std::cout << position << ' ';
}

std::string get_string() {
    char ch;
    std::string s;
    while (std::cin.get(ch)) {
        if (ch == '\n') {
            break;
        }
        if (ch == '?' || (ch >= 'a' && ch <= 'z')) {
            s.push_back(ch);
        }
    }
    return s;
}

int main() {
    std::string pattern;
    std::string text;
    pattern = get_string();
    text = get_string();
    Matcher matcher(pattern);
    matcher.find_matches(text, &print_answer);
}
