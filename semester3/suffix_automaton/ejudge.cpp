#include <string>
#include <iostream>

const int ALPHA_LENGTH = 26;

struct State {
    int length;
    State* suffix;
    State* transitions[ALPHA_LENGTH];
    int path_count;
    int reference_count;

    State(int length = 0, State* suffix = NULL) : reference_count(0), length(length), suffix(suffix), path_count(0) {
        for (int i = 0; i < ALPHA_LENGTH; ++i) {
            transitions[i] = NULL;
        }
    }

    State* clone() {
        State* clone = new State(length, suffix);
        for (int i = 0; i < ALPHA_LENGTH; ++i) {
            clone->transitions[i] = transitions[i];
            if (transitions[i]) {
                transitions[i]->reference_count++;
            }
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

    ~State() {
        for (int i = 0; i < ALPHA_LENGTH; ++i) {
            if (transitions[i]) {
                transitions[i]->reference_count--;
                if (transitions[i]->reference_count <= 0) {
                    delete transitions[i];
                }
            }
        }
    }

};

class Automaton {
    State initial;

public:
    void build(const std::string& str) {
        State* last = &initial;
        for (int i = 0; i < str.length(); ++i) {
            State* cur = new State(last->length + 1);
            short next_char = str[i] - 'a';
            while (true) {
                if (last == NULL) {
                    cur->suffix = &initial;
                    break;
                }
                State* target = last->transitions[next_char];
                if (target != NULL) {
                    if (target->length == last->length + 1) {
                        cur->suffix = target;
                        break;
                    }
                    State* clone = target->clone();
                    clone->length = last->length + 1;
                    cur->suffix = clone;
                    target->suffix = clone;
                    while (last != NULL && last->transitions[next_char] == target) {
                        last->transitions[next_char] = clone;
                        target->reference_count--;
                        clone->reference_count++;
                        last = last->suffix;
                    }
                    break;
                } else {
                    last->transitions[next_char] = cur;
                    cur->reference_count++;
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

int main() {
    std::string s;
    std::cin >> s;
    Automaton a;
    a.build(s);
    std::cout << a.count_strings() - 1;
}
