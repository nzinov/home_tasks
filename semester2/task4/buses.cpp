#include "../googletest/include/gtest/gtest.h"
#include <functional>
#include "dijkstra.cpp"

using namespace std::placeholders;

typedef long long time_type;

struct Route : GenericEdge {
    time_type departure;
    time_type duration;
    time_type interval;

    Route(size_t tail, size_t head, time_type departure, time_type duration, time_type interval) :
        GenericEdge(tail, head), departure(departure), duration(duration), interval(interval) {}
};

struct BusPath {
    time_type end_time;
    BusPath(time_type ends = 0) : end_time(ends) {}

    BusPath extend(const Route& route) {
        time_type departure = route.departure;
        while (departure < end_time) {
            departure += route.interval;
        }
        return BusPath(departure + route.duration);
    }

    bool operator<(const BusPath& other) {
        return end_time < other.end_time;
    }
};

struct Schedule {
    vector<vector<Route> > routes;

    Schedule(size_t n) : routes(n) {};

    const vector<Route>& get_routes(size_t v) {
        return routes[v];
    }

    void add_route(Route route) {
        routes[route.tail].push_back(route);
    };

};

TEST(DijkstraTest, Bus) {
    Schedule schedule(2);
    schedule.add_route(Route(0, 1, 5, 5, 3));
    1 1 2 4
    2 0 2 0
    3 2 2 2
    3 2 1 3
    1 1 2 2
    2 0 1 1
    auto solver = get_solver<Route, BusPath>(
        std::bind(&Schedule::get_routes, schedule, _1));
    solver.find_path(0, 1);
}
