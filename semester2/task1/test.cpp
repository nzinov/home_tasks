#include <vector>
#include <random>
#include <iostream>
#include "permutationstruct.h"
#include "simplestruct.h"
#include "treap.h"

using std::vector;

enum Command {
    INSERT,
    SET,
    SUM,
    NEXT_PERMUTATION
};

struct Test {
    Command command;
    size_t pos;
    size_t left;
    size_t right;
    int value;
};

vector<Test> generate_tests(unsigned int count, int range) {
    vector<Test> tests;
    size_t length = 0;
    for (unsigned int i = 0; i < count; ++i) {
        Command command;
        size_t pos, left, right;
        if (length == 0) {
            command = INSERT;
            pos = 0;
        } else {
            command = static_cast<Command>(rand() % 4);
            left = rand() % length;
            right = rand() % (length - left) + left + 1;
            pos = rand() % (length + 1);
        }
        tests.push_back(Test({command, pos, left, right, rand() % range}));
        if (command == INSERT) {
            ++length;
        }
    }
    return tests;
}

vector<long long> run_test(PermutationStruct* target, vector<Test> tests) {
    vector<long long> result;
    for (Test test : tests) {
        switch (test.command) {
            case INSERT:
                target->insert(test.value, test.pos);
                break;
            case SET:
                target->set(test.value, test.left);
                break;
            case SUM:
                result.push_back(target->sum(test.left, test.right));
                break;
            case NEXT_PERMUTATION:
                target->next_permutation(test.left, test.right);
                break;
        }
    }
    return result;
}

const char* names[] = {"Insert", "Set", "Sum", "Next"};

int main() {
    unsigned count;
    int range;
    std::cout << "Enter number of operations: ";
    std::cin >> count;
    std::cout << "Enter maximum value: ";
    std::cin >> range;
    srand(57);
    SimpleStruct simple;
    Treap treap;
    vector<Test> tests = generate_tests(count, range);
    vector<long long> test_result = run_test(&treap, tests);
    vector<long long> control_result = run_test(&simple, tests);
    if (std::mismatch(test_result.begin(), test_result.end(), control_result.begin()).first == test_result.end()) {
        std::cout << "OK";
    } else {
        std::cout << "Mismatch";
    }
}
