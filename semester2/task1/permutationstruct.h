class PermutationStruct {
public:
    virtual void insert(int value, size_t pos) = 0;
    virtual void set(int value, size_t pos) = 0;
    virtual long long sum(size_t left, size_t right) = 0;
    virtual void next_permutation(size_t left, size_t right) = 0;
};
