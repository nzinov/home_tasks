#include <cstddef>
#include <iterator>

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
        return array[get_index(index)];
    }

    T& operator[](size_t index) const {
        return at(index);
    }

    void resize() {
        if (size() + 1 == capacity) {
            capacity *= 2;
        } else if (size()*4 < capacity) {
            capacity /= 2;
        } else {
            return;
        }
        T* new_array = new T[capacity];
        for (size_t i = 0; i < size(); ++i) {
            new_array[i] = at(i);
        }
        delete[] array;
        array = new_array;
        begin_index = 0;
        end_index = size();
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
        resize();
        T val = array[begin_index];
        return val;
    }

    T pop_back() {
        resize();
        end_index = safe_add(end_index, -1);
        return  array[end_index];
    }

    T& back() {
        return array[end_index];
    }

    const T& back() const {
        return array[end_index];
    }

    T& front() {
        return array[begin_index];
    }

    const T& front() const {
        return array[begin_index];
    }

    template <bool is_const> class generic_iterator : public std::iterator<std::random_access_iterator_tag, T> {
        Deque* target;
        size_t index;

    public:
        typedef typename std::conditional<is_const, const T*, T*>::type pointer;
        typedef typename std::conditional<is_const, const T&, T&>::type reference;

        generic_iterator(Deque* target, size_t index) : target(target), index(index) {}

        bool operator==(const generic_iterator& b) const {
            return index == b.index;
        }

        bool operator!=(const generic_iterator& b) const {
            return index != b.index;
        }

        bool operator>(const generic_iterator& b) const {
            return index > b.index;
        }

        bool operator<(const generic_iterator& b) const {
            return index < b.index;
        }

        reference operator*() const {
            return target->at(index);
        }

        pointer operator->() const {
            return &(target->at(index));
        }

        generic_iterator operator+(ptrdiff_t diff) {
            return iterator(target, index + diff);
        }

        generic_iterator operator-(ptrdiff_t diff) {
            return iterator(target, index - diff);
        }

        ptrdiff_t operator-(generic_iterator& b) {
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

        generic_iterator operator++(int) {
            generic_iterator copy = *this;
            ++index;
            return copy;
        }

        generic_iterator operator--(int) {
            generic_iterator copy = *this;
            --index;
            return copy;
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

    const_iterator cbegin() {
        return const_iterator(this, 0);
    }

    const_iterator cend() {
        return const_iterator(this, size());
    }

    std::reverse_iterator<iterator> rbegin() {
        return std::reverse_iterator<iterator>(end());
    }

    std::reverse_iterator<iterator> rend() {
        return std::reverse_iterator<iterator>(begin());
    }

    std::reverse_iterator<const_iterator> crbegin() {
        return std::reverse_iterator<const_iterator>(end());
    }

    std::reverse_iterator<const_iterator> crend() {
        return std::reverse_iterator<const_iterator>(begin());
    }
};
