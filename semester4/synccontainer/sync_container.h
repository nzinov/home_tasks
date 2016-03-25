#include <mutex>
#include <condition_variable>
#include <queue>
#include <stack>
#include <experimental/optional>

using std::experimental::optional;

template <typename Container> class SyncContainer {
    Container data;
    std::mutex op;
    std::condition_variable not_empty;
    typedef typename Container::value_type T;

public:
    void push(T el);
    optional<T> pop_nowait();
    T pop();
};

template<typename Container> void SyncContainer<Container>::push(SyncContainer::T el) {
    std::unique_lock<std::mutex> lock(op);
    data.push_back(el);
    not_empty.notify_one();
}

template<typename Container> optional<typename SyncContainer<Container>::T> SyncContainer<Container>::pop_nowait() {
    std::unique_lock<std::mutex> lock(op);
    if (data.empty()) {
        return optional<T>();
    }
    T el = data.back();
    data.pop_back();
    return el;
}

template<typename Container> typename SyncContainer<Container>::T SyncContainer<Container>::pop() {
    std::unique_lock<std::mutex> lock(op);
    while (data.empty()) {
        not_empty.wait(lock);
    }
    T el = data.back();
    data.pop_back();
    return el;
}
