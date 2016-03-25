#include "sync_container.h"
#include <vector>
#include <list>
#include <deque>
#include <queue>
#include <stack>
#include <thread>
#include <mutex>
#include "gtest/gtest.h"

const int THREAD_NUM = 3;
const int SIZE = 100000;

template<typename T> struct SyncContainerTest : ::testing::Test {
    SyncContainer<T> obj;
    std::vector<int> used;

    SyncContainerTest() : used(THREAD_NUM*SIZE) {}
};

typedef ::testing::Types<std::vector<int>, std::deque<int>, std::list<int>, std::queue<int>, std::stack<int> > MyTypes;
TYPED_TEST_CASE(SyncContainerTest, MyTypes);

template<typename T> void test_push(SyncContainer<T>& cont, int shift) {
    for (int i = shift; i < shift + SIZE; ++i) {
        cont.push(i);
    }
}

template<typename T> void test_pop(SyncContainer<T>& cont, std::vector<int>& used) {
    for (int i = 0;i < SIZE; ++i) {
        int el =cont.pop();
        ASSERT_FALSE(used[el]);
        used[el] = true;
    }
}

TYPED_TEST(SyncContainerTest, Test) {
    std::vector<std::thread> threads;
    for (int i = 0; i < THREAD_NUM; ++i) {
        threads.emplace_back(test_push<TypeParam>, std::ref(this->obj), SIZE*i);
    }
    for (int i = 0; i < THREAD_NUM; ++i) {
        threads.emplace_back(test_pop<TypeParam>, std::ref(this->obj), std::ref(this->used));
    }
    for (std::thread& t : threads) {
        t.join();
    }
    for (int i = 0; i < THREAD_NUM*SIZE; ++i) {
        ASSERT_TRUE(this->used[i]);
    }
}
