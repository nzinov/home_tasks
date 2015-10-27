#ifndef LOG_H
#define LOG_H
#include <vector>
#include "action.h"
#include "graph.h"

struct LogEntry {
    Action action;
    Network network;

    LogEntry(Action action, Network network);
};

class Log {
    std::vector<LogEntry> states;
    std::vector<LogEntry>::const_iterator cursor;

public:
    Log();
    void add(Action action, Network network);
    const LogEntry& state();
    const Network& network();
    const Action& action();
    void increase();
    void decrease();
};
#endif
