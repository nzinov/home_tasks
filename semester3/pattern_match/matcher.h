#include <vector>
#include <iostream>
#include "aho.h"

class Matcher {
    const std::string& pattern;
    Trie trie;
    std::vector<int> offsets;
    int* counts;

public:
    Matcher(const std::string& pattern);
    ~Matcher() {
        std::cerr << "enter" << this;
        delete[] counts;
    };
    void process_occurence(int position, int block_id) {
        std::cerr << "calculate" << this;
        counts[position - offsets[block_id]]++;
    }
        
    void find_matches(const std::string& text, void (*callback)(int position));
};
