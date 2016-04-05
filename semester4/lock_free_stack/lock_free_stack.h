#include <iostream>
#include <atomic>
#include <experimental/optional>

using std::experimental::optional;

template<typename T> struct Node {
    std::size_t next;
    T data;
};

template<typename T> struct Ptr {
    Node<T>* ptr;
    int tag;

    Ptr(Node<T>* ptr, int tag) : ptr(ptr), tag(tag) {}
    
    operator std::size_t() {
        return reinterpret_cast<std::size_t>(ptr) | tag;
    }

    Ptr(const std::size_t& val) {
        ptr = reinterpret_cast<Node<T>*>(val & ~0x3);
        tag = val & 0x3;
    }
};

template<typename T> class LockFreeStack {
    std::atomic<std::size_t> head;
public:
    void push(T el);
    optional<T> pop();
    LockFreeStack() : head(0) {}
};

template<typename T> void LockFreeStack<T>::push(T el) {
    Node<T>* n = new Node<T>{head, el};
    int tag = Ptr<T>(head).tag;
    std::size_t new_head = Ptr<T>(n, (tag + 1) % 4);
    while (!head.compare_exchange_strong(n->next, new_head));
}

template<typename T> optional<T> LockFreeStack<T>::pop() {
    std::size_t ptr = head;
    while (true) {
        Node<T>* el = Ptr<T>(ptr).ptr;
        if (el == nullptr) {
            return optional<T>();
        }
        if (head.compare_exchange_strong(ptr, el->next)) {
            T data = el->data;
            delete el;
            return data;
        }
    }
}
