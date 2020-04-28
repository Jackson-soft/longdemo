#pragma once

// 同步队列
#include "noncopyable.hpp"
#include <mutex>
#include <queue>
#include <utility>

namespace Uranus::Utils
{
template<typename T>
class SyncQueue : public Noncopyable
{
public:
    SyncQueue()  = default;
    ~SyncQueue() = default;

    auto Empty() -> bool
    {
        std::lock_guard<std::mutex> locker(mutex);
        return queue.empty();
    }

    void Push(T &&value)
    {
        std::lock_guard<std::mutex> locker(mutex);
        queue.push(std::move(value));
    }

    void Push(const T &value)
    {
        std::lock_guard<std::mutex> locker(mutex);
        queue.push(value);
    }

    void Emplace(T &&value)
    {
        std::lock_guard<std::mutex> locker(mutex);
        queue.emplace(value);
    }

    void Pop()
    {
        std::lock_guard<std::mutex> locker(mutex);
        queue.pop();
    }

    auto Front()
    {
        std::lock_guard<std::mutex> locker(mutex);
        return queue.front();
    }

    auto Back()
    {
        std::lock_guard<std::mutex> locker(mutex);
        return queue.back();
    }

private:
    std::mutex mutex;
    std::queue<T> queue;
};
}  // namespace Uranus::Utils
