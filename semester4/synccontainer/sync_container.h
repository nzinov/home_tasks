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
    auto _push(typename Container::value_type el, int = 0) -> decltype(data.push(el), void());
    auto _push(typename Container::value_type el) -> decltype(data.push_back(el), void());
    auto _pop(int = 0) -> decltype(data.pop(), T());
    auto _pop() -> decltype(data.pop_back(), T());
    auto _top() -> decltype(data.top(), T());
    auto _top(int = 0) -> decltype(data.pop(), T());
    auto _top(long = 0) -> decltype(data.pop_back(), T());

public:
    void push(T el);
    optional<T> pop_nowait();
    T pop();
};

template<typename Container> auto SyncContainer<Container>::_push(typename Container::value_type el, int) -> decltype(data.push(el), void()){
    data.push(el);
}

template<typename Container> auto SyncContainer<Container>::_push(typename Container::value_type el) -> decltype(data.push_back(el), void()) {
    data.push_back(el);
}

template<typename Container> auto SyncContainer<Container>::_pop(int) -> decltype(data.pop(), T()) {
    T el = _top();
    data.pop();
    return el;
}

template<typename Container> auto SyncContainer<Container>::_pop() -> decltype(data.pop_back(), T()) {
    T el = _top();
    data.pop_back();
    return el;
}

template<typename Container> auto SyncContainer<Container>::_top() -> decltype(data.top(), T()) {
    return data.top();
}

template<typename Container> auto SyncContainer<Container>::_top(int) -> decltype(data.pop(), T()) {
    return data.front();
}

template<typename Container> auto SyncContainer<Container>::_top(long) -> decltype(data.pop_back(), T()) {
    return data.back();
}

template<typename Container> void SyncContainer<Container>::push(SyncContainer::T el) {
    std::unique_lock<std::mutex> lock(op);
    _push(el);
    not_empty.notify_one();
}

template<typename Container> optional<typename SyncContainer<Container>::T> SyncContainer<Container>::pop_nowait() {
    std::unique_lock<std::mutex> lock(op);
    if (data.empty()) {
        return optional<T>();
    }
    return _pop();
}

template<typename Container> typename SyncContainer<Container>::T SyncContainer<Container>::pop() {
    std::unique_lock<std::mutex> lock(op);
    while (data.empty()) {
        not_empty.wait(lock);
    }
    return _pop();
}
