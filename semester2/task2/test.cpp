#include "deque.h"
#include <gtest/gtest.h>
#include <deque>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <iostream>

const size_t CHUNK_SIZE = 1000000;
const size_t CHUNK_COUNT = 10;

class DequeTest : public testing::Test {
    protected:
    Deque<int> deque;
    std::vector<int> control;

    virtual void SetUp() {
        control = {1, 9, 5, 6, 3, 8 ,11, 57, 310, 228};
        std::copy(control.begin(), control.end(), std::back_inserter(deque));
    }
};

TEST_F(DequeTest, Sort) {
    std::sort(deque.begin(), deque.end());
    std::sort(control.begin(), control.end());
    EXPECT_TRUE(std::equal(control.begin(), control.end(), deque.begin()));
}

TEST_F(DequeTest, ReverseIterator) {
    EXPECT_TRUE(std::equal(control.rbegin(), control.rend(), deque.rbegin()));
}

TEST_F(DequeTest, ConstIterator) {
    EXPECT_TRUE(std::equal(control.begin(), control.end(), deque.cbegin()));
}

TEST_F(DequeTest, PopFront) {
    for (size_t i = 0; i < control.size(); ++i) {
        EXPECT_FALSE(deque.empty());
        EXPECT_EQ(deque[0], control[i]) << "i = " << i;
        EXPECT_EQ(deque.size(), control.size() - i);
        EXPECT_EQ(deque.pop_front(), control[i]);
    }
    EXPECT_EQ(deque.size(), 0);
    EXPECT_TRUE(deque.empty());
}

TEST_F(DequeTest, PopBack) {
    while (!control.empty()) {
        EXPECT_FALSE(deque.empty());
        EXPECT_EQ(deque.back(), control.back());
        EXPECT_EQ(deque.pop_back(), control.back());
        control.pop_back();
        EXPECT_EQ(deque.size(), control.size());
    }
    EXPECT_TRUE(deque.empty());
}

TEST(EmptyDequeTest, PushFront) {
    Deque<int> deque;
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(deque.size(), i);
        deque.push_front(i);
        EXPECT_EQ(deque[0], i);
        EXPECT_EQ(deque.size(), i+1);
    }
}

TEST_F(DequeTest, Access) {
    for (size_t i = 0; i < control.size(); ++i) {
        EXPECT_EQ(deque[i], control[i]);
    }
    EXPECT_EQ(deque.front(), control.front());
    EXPECT_EQ(deque.back(), control.back());
}

TEST(EmptyDequeTest, Random) {
    std::deque<int> control;
    Deque<int> deque;
    for (size_t i = 0; i < CHUNK_COUNT; ++i) {
        clock_t time = clock();
        for (size_t c = 0; c < CHUNK_SIZE; ++c) {
            int operand = rand();
            switch (rand() % 4) {
                case 0:
                    deque.push_back(operand);
                    control.push_back(operand);
                    break;
                case 1:
                    deque.push_front(operand);
                    control.push_front(operand);
                    break;
                case 2:
                    if (control.size() == 0) {
                        break;
                    }
                    EXPECT_EQ(deque.pop_front(), control.front());
                    control.pop_front();
                    break;
                case 3:
                    if (control.size() == 0) {
                        break;
                    }
                    EXPECT_EQ(deque.pop_back(), control.back());
                    control.pop_back();
                    break;
            }
        }
        std::cout << clock() - time << std::endl;
    }
}


