#include <atomic>
#include <experimental/optional>

using std::experimental::optional;

template<typename T> struct Node {
    Node* next;
    T data;
};

template<typename T> class LockFreeStack {
    std::atomic<Node<T>*> head;
public:
    void push(T el);
    optional<T> pop();
    LockFreeStack() : head(nullptr) {}
};

template<typename T> void LockFreeStack<T>::push(T el) {
    Node<T>* n = new Node<T>{head, el};
    while (!head.compare_exchange_strong(n->next, n));
}

template<typename T> optional<T> LockFreeStack<T>::pop() {
    Node<T>* el = head;
    while (true) {
        if (el == nullptr) {
            return optional<T>();
        }
        if (head.compare_exchange_strong(el, el->next)) {
            T data = el->data;
            delete el;
            return data;
        }
    }
}
