#pragma once

#include "Util.hpp"
#include <mutex>
#include <shared_mutex>

// 环形缓存
template <typename T>
class RingBuffer : public Noncopyable
{
public:
	RingBuffer() {}
	~RingBuffer() {}

	//读取
	T Pop() { std::shared_lock<std::shared_mutex> lock(mMutex); }

	// 写入
	void Push() { std::unique_lock<std::shared_mutex> lock(mMutex); }

private:
	mutable std::shared_mutex mMutex;
	int mMaxSize;
};