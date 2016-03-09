#include <thread>
#include <vector>
#include <iostream>
#include <algorithm>
#include <mutex>
#include <chrono>

const int COUNT = 100000000;

std::mutex add;
int accumulator;

void sum(std::vector<int>::iterator begin, std::vector<int>::iterator end) {
    int ans = std::accumulate(begin, end, 0);
    add.lock();
    accumulator += ans;
    add.unlock();
}
    
int main() {
    int currentX = 1;
    std::vector<int> X;
    auto it = back_inserter(X);
    X.reserve(COUNT);
    std::generate_n(it, COUNT, [&] () -> int {currentX = currentX * 1664525 + 1013904223; return currentX;});
    for (size_t thread_count = 1; thread_count < 12; ++thread_count) {
        auto start = std::chrono::steady_clock::now();
        std::vector<std::thread> threads;
        size_t len = X.size() / thread_count;
        size_t current = 0;
        accumulator = 0;
        for (size_t i = 0; i < thread_count; ++i) {
            int end = current + len;
            if (i + 1 == thread_count) {
                end = X.size();
            }
            threads.push_back(std::thread(sum, X.begin() + current, X.begin() + end));
            current = end;
        }
        for (std::thread& t : threads) {
            t.join();
        }
        int delta = (std::chrono::steady_clock::now() - start).count();
        std::cout << thread_count << ' ' << delta << std::endl;
        std::cerr << accumulator << std::endl;
    }
}
