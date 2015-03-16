#include <vector>
#include "permutationstruct.h"

class SimpleStruct : public PermutationStruct {
public:
    virtual void insert(int value, size_t pos) override;
    virtual void set(int value, size_t pos) override;
    virtual long long sum(size_t left, size_t right) override;
    virtual void next_permutation(size_t left, size_t right) override;
private:
    std::vector<int> data;
};
