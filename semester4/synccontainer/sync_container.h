#include <mutex>
#include <condition_variable>
#include <queue>
#include <stack>

struct EmptyContainer {
};

template <typename Container> class Adapter {
    Container data;
    typedef typename Container::value_type T;

    void push_back(T el);
    T pop_back();
    bool empty();
};

template <typename Container> class SyncContainer {
    Container data;
    std::mutex op;
    std::condition_variable not_empty;
    typedef typename Container::value_type T;

    void push(T el);
    T pop_nowait();
    T pop();
};

template<typename Container> void Adapter::push_back(Adapter::T el) {
    data.push(el);
}

template<typename Container> T Adapter::

template<typename Container> void SyncContainer<Container>::push(SyncContainer::T el) {
    std::unique_lock<std::mutex> lock(op);
    data.push_back(el);
    not_empty.notify_all();
}

template<typename Container> typename SyncContainer<Container>::T SyncContainer<Container>::pop_nowait() {
    std::unique_lock<std::mutex> lock(op);
    if (data.empty()) {
        throw EmptyContainer();
    }
    return data.pop_back();
}

template<typename Container> typename SyncContainer<Container>::T SyncContainer<Container>::pop() {
    std::unique_lock<std::mutex> lock(op);
    while (data.empty()) {
        not_empty.wait(lock);
    }
    return data.pop_back();
}
