#pragma once

// 同步队列

#include "utils/Util.hpp"
#include <mutex>
#include <queue>
#include <utility>

namespace Uranus
{
template<typename T>
class SyncQueue : public Utils::Noncopyable
{
public:
    SyncQueue()  = default;
    ~SyncQueue() = default;

    bool Empty()
    {
        std::lock_guard<std::mutex> locker(mMutex);
        return mQueue.empty();
    }

    void Push(T &&value)
    {
        std::lock_guard<std::mutex> locker(mMutex);
        mQueue.push(std::move(value));
    }

    void Push(const T &value)
    {
        std::lock_guard<std::mutex> locker(mMutex);
        mQueue.push(value);
    }

    void Emplace(T &&value)
    {
        std::lock_guard<std::mutex> locker(mMutex);
        mQueue.emplace(value);
    }

    void Pop()
    {
        std::lock_guard<std::mutex> locker(mMutex);
        mQueue.pop();
    }

    T &Front()
    {
        std::lock_guard<std::mutex> locker(mMutex);
        return mQueue.front();
    }

    T &Back()
    {
        std::lock_guard<std::mutex> locker(mMutex);
        return mQueue.back();
    }

private:
    std::mutex mMutex;
    std::queue<T> mQueue;
};
}  // namespace Uranus