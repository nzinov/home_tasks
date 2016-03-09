#include <iostream>
#include <vector>
#include <thread>
#include <atomic>

const std::size_t EMPTY_HASH = std::hash<std::thread::id>()(std::thread::id());

class Futex {
    std::atomic<std::size_t> data;

    Futex(Futex&) = delete;

public:
    Futex() {
        data.store(EMPTY_HASH);
    }

    void lock() {
        std::size_t newVal = std::hash<std::thread::id>()(std::this_thread::get_id());
        for (std::size_t desired = EMPTY_HASH; !data.compare_exchange_strong(desired, newVal); desired = EMPTY_HASH);
    }

    void unlock() {
        std::size_t desired = std::hash<std::thread::id>()(std::this_thread::get_id());
        data.compare_exchange_strong(desired, EMPTY_HASH);
    }
};

int counter = 0;
Futex mutex;

void add(int& local) {
    while (true) {
        mutex.lock();
        if (counter >= 10000000) {
            break;
        }
        counter++;
        local++;
        mutex.unlock();
    }
}

int main() {
    std::vector<std::thread> threads;
    const int THREAD_NUM = 4;
    std::vector<int> counters(THREAD_NUM);
    for (int i = 0; i < THREAD_NUM; ++i) {
        threads.emplace_back(add, std::ref(counters[i]));
    }
    for (auto& thread : threads) {
        thread.join();
    }
    for (auto el : counters) {
        std::cout << el << ' ';
    }
    std::cout << std::endl;
    std::cout << counter;
}
