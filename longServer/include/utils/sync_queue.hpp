#pragma once

// 同步队列

#include "noncopyable.hpp"

#include <mutex>
#include <queue>
#include <utility>

namespace uranus::utils {
template<typename T>
class SyncQueue : public Noncopyable
{
public:
    SyncQueue()  = default;
    ~SyncQueue() = default;

    auto Empty() -> bool {
        std::lock_guard<std::mutex> locker(mutex_);
        return queue_.empty();
    }

    void Push(T &&value) {
        std::lock_guard<std::mutex> locker(mutex_);
        queue_.push(std::move(value));
    }

    void Push(const T &value) {
        std::lock_guard<std::mutex> locker(mutex_);
        queue_.push(value);
    }

    void Emplace(T &&value) {
        std::lock_guard<std::mutex> locker(mutex_);
        queue_.emplace(value);
    }

    void Pop() {
        std::lock_guard<std::mutex> locker(mutex_);
        queue_.pop();
    }

    auto Front() -> T & {
        std::lock_guard<std::mutex> locker(mutex_);
        return queue_.front();
    }

    auto Back() -> T & {
        std::lock_guard<std::mutex> locker(mutex_);
        return queue_.back();
    }

private:
    std::mutex    mutex_;
    std::queue<T> queue_;
};
}  // namespace uranus::utils