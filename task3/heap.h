#ifndef HEAP_HEADER
#define HEAP_HEADER
typedef unsigned int key_type;

struct EmptyHeapException {
};

struct InconsistentTypeException {
};

class Heap {
public:
    virtual key_type get_min() const = 0;
    virtual key_type extract_min() = 0;
    virtual void insert(key_type) = 0;
    virtual void merge(Heap*) = 0;
    virtual bool is_empty() const = 0;
    virtual ~Heap() {};
};
#endif
