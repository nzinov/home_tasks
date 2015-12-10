#include <string>

const int ALPHA_LENGTH = 26;

struct State {
    int length;
    State* suffix;
    State* transitions[ALPHA_LENGTH];
    int path_count;

    State(int length = 0, State* suffix = nullptr) : length(length), suffix(suffix), path_count(0) {
        for (int i = 0; i < ALPHA_LENGTH; ++i) {
            transitions[i] = nullptr;
        }
    }

    State* clone() {
        State* clone = new State(length, suffix);
        for (int i = 0; i < ALPHA_LENGTH; ++i) {
            clone->transitions[i] = transitions[i];
        }
        return clone;
    }

    int count_paths() {
        if (path_count == 0) {
            path_count = 1;
            for (int i = 0; i < ALPHA_LENGTH; ++i) {
                if (transitions[i]) {
                    path_count += transitions[i]->count_paths();
                }
            }
        }
        return path_count;
    }
};

class Automaton {
    State initial;

public:
    void build(const std::string& str) {
        State* last = &initial;
        for (int i = 0; i < static_cast<int>(str.length()); ++i) {
            State* cur = new State(last->length + 1);
            short next_char = str[i] - 'a';
            while (true) {
                if (last == nullptr) {
                    cur->suffix = &initial;
                    break;
                }
                State* target = last->transitions[next_char];
                if (target != nullptr) {
                    if (target->length == last->length + 1) {
                        cur->suffix = target;
                        break;
                    }
                    State* clone = target->clone();
                    clone->length = last->length + 1;
                    cur->suffix = clone;
                    target->suffix = clone;
                    while (last != nullptr && last->transitions[next_char] == target) {
                        last->transitions[next_char] = clone;
                        last = last->suffix;
                    }
                    break;
                } else {
                    last->transitions[next_char] = cur;
                    last = last->suffix;
                }
            }
            last = cur;
        }
    };

    int count_strings() {
       return initial.count_paths();
    }
};
