#include <vector>
#include "permutationstruct.h"

class SimpleStruct : public PermutationStruct {
public:
    virtual void insert(int value, size_t pos);
    virtual void set(int value, size_t pos);
    virtual long long sum(size_t left, size_t right);
    virtual void next_permutation(size_t left, size_t right);
private:
    std::vector<int> data;
};
