#include <functional>
#include "matcher.h"

Matcher::Matcher(const std::string& pattern) : pattern(pattern), counts(pattern.length()) {
    int position = 0;
    for (int i = 0; i < pattern.length(); ++i) {
        if (pattern[i] == '?') {
            if (i > position) {
                offsets.push_back(position);
                trie.add_string(pattern, position, i, offsets.size() - 1);
            }
            position = i + 1;
        }
    }
    if (position < pattern.length()) {
        offsets.push_back(position);
        trie.add_string(pattern, position, pattern.length(), offsets.size() - 1);
    }
    trie.finalize();
}

void Matcher::find_matches(const std::string& text, void (*callback)(int position)) {
    using namespace std::placeholders;
   trie.process(text, std::bind(&Matcher::process_occurence, this, _1, _2)); 
   for (int i = 0; i < text.length(); ++i) {
       if (counts[i] == end_offsets.size()) {
           callback(i);
       }
   }
}
