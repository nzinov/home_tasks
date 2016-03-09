#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>

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

template<typename Mutex> class Lock {
    Mutex* mutex;
public:
    Lock(Mutex* mutex) : mutex(mutex) {
        mutex->lock();
    }

    ~Lock() {
        mutex->unlock();
    }
};

const std::size_t TARGET = 10000000;

template<typename Mutex> void add(std::size_t& local, std::size_t& global, Mutex* mutex) {
    while (true) {
        mutex->lock();
        if (global >= TARGET) {
            mutex->unlock();
            break;
        }
        global++;
        mutex->unlock();
        local++;
    }
}

template<typename Mutex> void test(std::size_t thread_num) {
    std::vector<std::thread> threads;
    std::vector<std::size_t> counters(thread_num);
    std::size_t counter = 0;
    Mutex mutex;
    auto start = std::chrono::steady_clock::now();
    for (std::size_t i = 0; i < thread_num; ++i) {
        threads.emplace_back(add<Mutex>, std::ref(counters[i]), std::ref(counter), &mutex);
    }
    for (auto& thread : threads) {
        thread.join();
    }
    std::chrono::milliseconds delta = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
    std::cout << "thread-local counts: ";
    std::size_t sum = 0;
    for (auto el : counters) {
        std::cout << el << ' ';
        sum += el;
    }
    std::cout << std::endl;
    std::cout << "sum of counters: " << sum << std::endl;
    std::cout << "global counter: " << counter << std::endl;
    std::cout << "time: " << delta.count() << std::endl;
}

int main() {
    for (std::size_t thread_num : {std::thread::hardware_concurrency() / 2, std::thread::hardware_concurrency() * 2}) {
        test<Futex>(thread_num);
        test<std::mutex>(thread_num);
    }
}
