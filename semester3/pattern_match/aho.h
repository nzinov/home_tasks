#include <string>
#include <vector>
#include<functional>

static const int ALPABET_LENGTH = 26;

struct Node {
    Node* suffix;
    Node* children[ALPABET_LENGTH];
    bool terminal;
    std::vector<short> substring_id;

    Node();
    ~Node();
    inline short index(char ch);
    Node* go(short mark);
    Node* go(char mark);
    Node* force_go(char mark);
    void calculate_links(Node* link = nullptr);
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
