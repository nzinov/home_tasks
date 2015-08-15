#include "../googletest/include/gtest/gtest.h"
#include <functional>
#include <iostream>
#include "dijkstra.cpp"

using namespace std::placeholders;

typedef long long cost_t;

struct Road : GenericEdge {
    const vector<cost_t>& prices;
    Road(size_t tail, size_t head, const vector<cost_t>& prices) : GenericEdge(tail, head), prices(prices) {}
};

struct Roadmap {
    vector<vector<Road> > roads;
    vector<cost_t> gas_price;

    Roadmap(size_t vertex_number) : roads(vertex_number), gas_price(vertex_number) {}

    const vector<Road>& get_edges(size_t v) {
        return roads[v];
    }

    void add_road(size_t a, size_t b) {
        roads[a].push_back(Road(a, b, gas_price));
        roads[b].push_back(Road(b, a, gas_price));
    };
};

struct Route {
    cost_t cost;
    Route(cost_t cost = 0) : cost(cost) {}

    Route extend(const Road& road) {
        return Route(cost + road.prices[road.tail]);
    }

    bool operator<(const Route& other) {
        return cost < other.cost;
    }
};

TEST(DijkstraTest, Refuel) {
    Roadmap roadmap(7);
    roadmap.gas_price = {1, 1, 1, 2, 2, 3, 3};
    roadmap.add_road(1, 2);
    roadmap.add_road(1, 3);
    roadmap.add_road(1, 6);
    roadmap.add_road(2, 3);
    roadmap.add_road(2, 4);
    roadmap.add_road(3, 6);
    roadmap.add_road(3, 4);
    roadmap.add_road(6, 5);
    roadmap.add_road(4, 5);
    auto solver = get_solver<Road, Route>(
        std::bind(&Roadmap::get_edges, roadmap, _1));
    auto answer = solver.find_path(1, 5);
    ASSERT_EQ(answer.best_path.cost, 4);
    const vector<size_t> TEST_ANSWER = {1, 2, 4, 5}; 
    EXPECT_TRUE(std::equal(TEST_ANSWER.begin(), TEST_ANSWER.end(), answer.vertices.begin()));
}
