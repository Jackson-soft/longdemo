#pragma once

#include <mutex>
#include <queue>
#include <utility>

template <typename T>
class SyncQueue
{
public:
	SyncQueue() {}
	~SyncQueue() {}

	bool Empty()
	{
		std::lock_guard<std::mutex> locker(tMutex);
		return mQueue.empty();
	}

	void Push(T &&value)
	{
		std::lock_guard<std::mutex> locker(tMutex);
		mQueue.push(std::move(value));
	}

	void Push(const T &value)
	{
		std::lock_guard<std::mutex> locker(tMutex);
		mQueue.push(value);
	}

    void Emplace(T && value)
    {
        std::lock_guard<std::mutex> locker(tMutex);
        mQueue.emplace(value);
    }

	void Pop()
	{
		std::lock_guard<std::mutex> locker(tMutex);
		mQueue.pop();
	}

	T &Front()
	{
		std::lock_guard<std::mutex> locker(tMutex);
        return mQueue.front();
	}

    T& Back()
    {
        std::lock_guard<std::mutex> locker(tMutex);
        return mQueue.back();
    }

private:
	std::mutex tMutex;
	std::queue<T> mQueue;
};
