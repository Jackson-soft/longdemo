#pragma once

#include <mutex>
#include <queue>

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

	void sd() {}

private:
	std::mutex tMutex;
	std::queue<T> mQueue;
};
