#include "deque.h"
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>

class DequeTest : public testing::Test {
    protected:
    Deque<int> deque;
    std::vector<int> control;

    virtual void SetUp() {
        control = {1, 9, 5, 6, 3, 8 ,11, 57, 310, 228};
        std::copy(control.begin(), control.end(), std::back_inserter(deque));
    }
};

TEST_F(DequeTest, TestSort) {
    std::sort(deque.begin(), deque.end());
    std::sort(control.begin(), control.end());
    EXPECT_TRUE(std::equal(deque.begin(), deque.end(), control.begin()));
}
