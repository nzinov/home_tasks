#include "log.h"

LogEntry::LogEntry(Action action, Network network) :
        action(action),
        network(network) {}

Log::Log() : cursor(states.begin()) {}

void Log::add(Action action, Network network) {
    states.emplace_back(action, network);
}

const LogEntry& Log::state() {
    return *cursor;
}

const Network& Log::network() {
    return state().network;
}

const Action& Log::action() {
    return state().action;
}

void Log::increase() {
    ++cursor;
}

void Log::decrease() {
    --cursor;
}
