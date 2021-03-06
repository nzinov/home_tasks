#include <vector>
#include "aho.h"

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
