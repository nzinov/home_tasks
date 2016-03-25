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

template<typename Container> auto _top(Container& cont, int) -> decltype(cont.top(), typename Container::value_type()) {
    return cont.top();
}

template<typename Container> auto _top(Container& cont, long) -> decltype(cont.pop(), typename Container::value_type()) {
    return cont.front();
}

template<typename Container> auto _top(Container& cont, long long) -> decltype(cont.pop_back(), typename Container::value_type()) {
    return cont.back();
}

template<typename Container> auto _push(Container& cont, typename Container::value_type el, int) -> decltype(cont.push(el), void()){
    cont.push(el);
}

template<typename Container> auto _push(Container& cont, typename Container::value_type el, long) -> decltype(cont.push_back(el), void()) {
    cont.push_back(el);
}

template<typename Container> auto _pop(Container& cont, int) -> decltype(cont.pop(), typename Container::value_type()) {
    typename Container::value_type el = _top(cont, 0);
    cont.pop();
    return el;
}

template<typename Container> auto _pop(Container& cont, long) -> decltype(cont.pop_back(), typename Container::value_type()) {
    typename Container::value_type el = _top(cont, 0);
    cont.pop_back();
    return el;
}

template<typename Container> void SyncContainer<Container>::push(SyncContainer::T el) {
    std::unique_lock<std::mutex> lock(op);
    _push(data, el, 0);
    not_empty.notify_one();
}

template<typename Container> optional<typename SyncContainer<Container>::T> SyncContainer<Container>::pop_nowait() {
    std::unique_lock<std::mutex> lock(op);
    if (data.empty()) {
        return optional<T>();
    }
    return _pop(data, 0);
}

template<typename Container> typename SyncContainer<Container>::T SyncContainer<Container>::pop() {
    std::unique_lock<std::mutex> lock(op);
    while (data.empty()) {
        not_empty.wait(lock);
    }
    return _pop(data, 0);
}
