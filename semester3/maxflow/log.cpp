#include "log.h"

LogEntry::LogEntry(Action action, Network network) :
        action(action),
        network(network) {}

Log::Log() {
}

void Log::add(Action action, const Network* network) {
    states.emplace_back(action, *network);
    cursor = states.cbegin();
}

const LogEntry& Log::state() {
    return *cursor;
}

const Network& Log::network() {
    return cursor->network;
}

const Action& Log::action() {
    return cursor->action;
}

void Log::step(short direction) {
    if (cursor == states.begin() && direction < 0) {
        return;
    }
    if (cursor + direction == states.end()) {
        return;
    }
    cursor += direction;
}
