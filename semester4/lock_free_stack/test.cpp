#include "lock_free_stack.h"
#include <vector>
#include <thread>
#include "gtest/gtest.h"

const int THREAD_NUM = 3;
const int SIZE = 10000000;

void test_push(LockFreeStack<int>& stack, int shift) {
    for (int i = shift; i < shift + SIZE; ++i) {
        stack.push(i);
    }
}

void test_pop(LockFreeStack<int>& stack, std::vector<int>& used) {
    for (int i = 0; i < SIZE; ) {
        optional<int> el = stack.pop();
        if (el) {
            ASSERT_FALSE(used[el.value()]) << "i = " << el.value();
            used[el.value()] = true;
            ++i;
        }
    }
}

TEST(LockFreeStackTest, Test) {
    std::vector<std::thread> threads;
    std::vector<int> used(SIZE*THREAD_NUM, 0);
    LockFreeStack<int> obj;
    for (int i = 0; i < THREAD_NUM; ++i) {
        threads.emplace_back(test_push, std::ref(obj), SIZE*i);
    }
    for (int i = 0; i < THREAD_NUM; ++i) {
        threads.emplace_back(test_pop, std::ref(obj), std::ref(used));
    }
    for (std::thread& t : threads) {
        t.join();
    }
    for (int i = 0; i < THREAD_NUM*SIZE; ++i) {
        ASSERT_TRUE(used[i]) << "i = " << i;
    }
}
