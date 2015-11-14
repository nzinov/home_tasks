#include <vector>
#include <string>
#include<functional>
#include <iostream>

static const int ALPABET_LENGTH = 26;

struct Node {
    Node* suffix;
    Node* children[ALPABET_LENGTH];
    bool terminal;
    short substring_id;

    Node();
    ~Node();
    inline short index(char ch);
    Node* go(short mark);
    Node* go(char mark);
    Node* force_go(char mark);
    void calculate_links(Node* link = NULL);
};

class Trie {
    Node root_node;

    public:
    Trie() {}
    ~Trie() {}
    void add_string(const std::string& str, int begin, int end, short id);
    void finalize();
    void process(const std::string& text, std::function<void(int position, int substring_id)> callback);
};

using std::string;

Node::Node() : suffix(NULL), terminal(false) {
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
    cursor->substring_id = id;
};

void Trie::finalize() {
    root_node.calculate_links();
}

void Trie::process(const string& text, std::function<void(int position, int substring_id)> callback) {
    Node* current_state = &root_node;
    for (int i = 0; i < text.length(); ++i) {
        current_state = current_state->go(text[i]);
        if (current_state->terminal) {
            callback(i, current_state->substring_id);
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
    trie.finalize();
}

void Matcher::find_matches(const std::string& text, void (*callback)(int position)) {
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


std::string get_string() {
    char ch;
    std::string s;
    while (std::cin.get(ch)) {
        if (ch == '\n') {
            if (s.length() > 0) {
                break;
            }
        }
        if ((ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9')) {
            abort();
        }
        if (ch == '?' || (ch >= 'a' && ch <= 'z')) {
            s.push_back(ch);
        }
    }
    return s;
}

void print_answer(int position) {
    std::cout << position << ' ';
}

int main() {
    std::string pattern;
    std::string text;
    pattern = get_string();
    text = get_string();
    Matcher matcher(pattern);
    matcher.find_matches(text, &print_answer);
}
