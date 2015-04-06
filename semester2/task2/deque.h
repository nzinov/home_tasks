#include <cstddef>
#include <iterator>
#include <boost/operators.hpp>

struct EmptyDequeException {
};

struct OutOfRangeException {
};

template <typename T> class Deque {
    T *array;
    size_t capacity;
    size_t begin_index;
    size_t end_index;

public:
    typedef T value_type;

    Deque() : capacity(1), begin_index(0), end_index(0) {
        array = new T[1];
    }

    ~Deque() {
        delete[] array;
    }

    size_t get_index(size_t i) const {
        return safe_add(begin_index, i);
    }

    size_t size() const {
        return safe_add(end_index, -static_cast<ptrdiff_t>(begin_index));
    }

    bool empty() const {
        return size() == 0;
    }

    size_t safe_add(size_t a, ptrdiff_t b) const {
        return (capacity + a + b) % capacity;
    }

    T& at(size_t index) const {
        if (index >= size()) {
            throw new OutOfRangeException;
        }
        return array[get_index(index)];
    }

    T& operator[](size_t index) {
        return at(index);
    }

    const T& operator[](size_t index) const {
        return at(index);
    }

    void resize() {
        size_t new_capacity;
        if (size() + 1 == capacity) {
            new_capacity = capacity * 2;
        } else if (size()*4 < capacity) {
            new_capacity = capacity / 2 + 1;
        } else {
            return;
        }
        T* new_array = new T[new_capacity];
        size_t current_size = size();
        for (size_t i = 0; i < size(); ++i) {
            new_array[i] = at(i);
        }
        delete[] array;
        array = new_array;
        capacity = new_capacity;
        begin_index = 0;
        end_index = current_size;
    }

    void push_front(T value) {
        resize();
        begin_index = safe_add(begin_index, -1);
        array[begin_index] = value;
    }

    void push_back(T value) {
        resize();
        array[end_index] = value;
        end_index = safe_add(end_index, 1);
    }

    T pop_front() {
        if (empty()) {
            throw new EmptyDequeException();
        }
        resize();
        T val = array[begin_index];
        begin_index = safe_add(begin_index, 1);
        return val;
    }

    T pop_back() {
        if (empty()) {
            throw new EmptyDequeException();
        }
        resize();
        end_index = safe_add(end_index, -1);
        return  array[end_index];
    }

    T& back() {
        return array[safe_add(end_index, -1)];
    }

    const T& back() const {
        return array[safe_add(end_index, -1)];
    }

    T& front() {
        return array[begin_index];
    }

    const T& front() const {
        return array[begin_index];
    }

    template <bool is_const> class generic_iterator : public boost::random_access_iterator_helper<Deque<T>::generic_iterator<is_const>, T, ptrdiff_t,
        typename std::conditional<is_const, const T*, T*>::type,
        typename std::conditional<is_const, const T&, T&>::type> {
    public:
        typedef typename std::conditional<is_const, const Deque*, Deque*>::type ptr_t;
        typedef typename std::conditional<is_const, const T*, T*>::type pointer;
        typedef typename std::conditional<is_const, const T&, T&>::type reference;
    private:
        ptr_t target;
        size_t index;
    public:
        generic_iterator(ptr_t target, size_t index) : target(target), index(index) {}

        bool operator==(const generic_iterator& b) const {
            return index == b.index;
        }

        bool operator<(const generic_iterator& b) const {
            return index < b.index;
        }

        reference operator*() const {
            return target->at(index);
        }

        generic_iterator& operator+=(ptrdiff_t diff) {
            index += diff;
            return *this;
        }

        generic_iterator& operator-=(ptrdiff_t diff) {
            index -= diff;
            return *this;
        }

        ptrdiff_t operator-(const generic_iterator& b) {
            return index - b.index;
        }

        generic_iterator& operator++() {
            ++index;
            return *this;
        }

        generic_iterator& operator--() {
            --index;
            return *this;
        }
    };

    typedef generic_iterator<false> iterator;

    typedef generic_iterator<true> const_iterator;

    iterator begin() {
        return iterator(this, 0);
    }

    iterator end() {
        return iterator(this, size());
    }

    const_iterator cbegin() const {
        return const_iterator(this, 0);
    }

    const_iterator cend() const {
        return const_iterator(this, size());
    }

    const_iterator begin() const {
        return cbegin();
    }

    const_iterator end() const {
        return cend();
    }

    std::reverse_iterator<iterator> rbegin() {
        return std::reverse_iterator<iterator>(end());
    }

    std::reverse_iterator<iterator> rend() {
        return std::reverse_iterator<iterator>(begin());
    }

    std::reverse_iterator<const_iterator> crbegin() {
        return std::reverse_iterator<const_iterator>(cend());
    }

    std::reverse_iterator<const_iterator> crend() {
        return std::reverse_iterator<const_iterator>(cbegin());
    }

    std::reverse_iterator<const_iterator> rbegin() const {
        return crbegin();
    }

    std::reverse_iterator<const_iterator> rend() const {
        return crend();
    }
};
