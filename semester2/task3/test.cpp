#include <vector>
#include <string>
#include <algorithm>
#include <gtest/gtest.h>
#include <iostream>

using std::vector;
using std::string;

vector<vector<size_t> > solution(string a, string b);
vector<vector<size_t> > stupid_solution(string a, string b);

const size_t MAX_LENGTH = 1000;
const size_t TEST_COUNT = 1000;

string random_string()
{
    auto randchar = []() -> char
        {
            const char charset[] = "XYZWxyzw57abcdefghijklmnopqrstuv";
            const size_t max_index = (sizeof(charset) - 1);
            return charset[rand() % max_index];
        };
    size_t length = rand() % MAX_LENGTH;
    std::string str(length, 0);
    std::generate_n( str.begin(), length, randchar);
    return str;
}

TEST(WeirdProblemTest, FromArticle) {
    string a = "yxxyzyzx";
    string b = "yxxyzxyzxyxzx";
    auto reference = stupid_solution(a, b);
    auto test = solution(a, b);
    size_t size = b.length() + 1;
    for (size_t i = 0; i < b.length(); ++i) {
        for (size_t j = 0; j < b.length(); ++j) {
            std::cout << reference[i][j] << " ";
        }
        std::cout << std::endl;
    }
    ASSERT_EQ(test.size(), size) << "vertical size mismatch";
    for (size_t i = 0; i < size; ++i) {
        ASSERT_EQ(test[i].size(), size) << "horizontal size mismatch in row " << i;
        for (size_t j = 0; j < size; ++j) {
            ASSERT_EQ(test[i][j], reference[i][j]) << "mismatch at " << i << ":" << j;
        }
    }
}

TEST(WeirdProblemTest, Random) {
    srand(57);
    for (size_t i = 0; i < TEST_COUNT; ++i) {
        string a = random_string();
        string b = random_string();
        auto reference = stupid_solution(a, b);
        auto test = solution(a, b);
        size_t size = b.length() + 1;
        ASSERT_EQ(test.size(), size) << "vertical size mismatch";
        for (size_t i = 0; i < size; ++i) {
            ASSERT_EQ(test[i].size(), size) << "horizontal size mismatch in row " << i;
            for (size_t j = 0; j < size; ++j) {
                ASSERT_EQ(test[i][j], reference[i][j]) << "mismatch at " << i << ":" << j;
            }
        }
    }
}
