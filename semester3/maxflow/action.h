#ifndef ACTION_H
#define ACTION_H
#include <cstddef>

struct Push {
    size_t source;
    size_t target;
};

struct Relabel {
    size_t vertex;
};

struct Gap {
    size_t height;
};

struct Action {
    enum {NONE, PUSH, RELABEL, GAP} tag;
    union {
        Push push;
        Relabel relabel;
        Gap gap;
    };

    Action() : tag(NONE) {}
    Action(Push push) : tag(PUSH), push(push) {}
    Action(Relabel relabel) : tag(RELABEL), relabel(relabel) {}
    Action(Gap gap) : tag(GAP), gap(gap) {}
};
#endif
