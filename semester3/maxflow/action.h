#ifndef ACTION_H
#define ACTION_H
#include <cstddef>
#include <QString>

struct Push {
    size_t source;
    size_t target;
    long long flow;
};

struct Relabel {
    size_t vertex;
    size_t height;
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
    QString message() const {
        switch (tag) {
            case NONE:
                return QString();
            case PUSH:
                return QString("push %1 units from v%2 to v%3").arg(push.flow).arg(push.source).arg(push.target);
            case RELABEL:
                return QString("relabel v%1 to height %2").arg(relabel.vertex).arg(relabel.height);
            case GAP:
                return QString("fire gap heuristic for height %1").arg(gap.height);
        }
    }
};
#endif
