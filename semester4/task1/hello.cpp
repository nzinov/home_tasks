#include <thread>
#include <iostream>
#include <vector>
#include <mutex>

std::mutex output;

int main() {
    std::vector<std::thread> threads;
    threads.reserve(10);
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([i] () {output.lock(); std::cout << "Hello Concurrent World! " << i << std::endl; output.unlock();});
    }
    for (std::thread& t : threads) {
        t.join();
    }
}




