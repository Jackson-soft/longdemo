#pragma once
//环形缓冲区

#include <mutex>
#include <shared_mutex>
#include <vector>

template <typename T>
class RingBuffer
{
public:
	RingBuffer() : RingBuffer(30) {}
	RingBuffer(int size) : nSize(size)
	{
		nWrite = nRead = 0;
		vElems.reserve(size);
	}
	~RingBuffer() { vElems.shrink_to_fit(); }

	void Push(const T &value)
	{
		//添加
		std::unique_lock<std::shared_mutex> lock(tMutex);
		return vElems.emplace_back(value);
	}

private:
	//读写锁
	mutable std::shared_mutex tMutex;
	unsigned int nSize; // 总容量
	unsigned int nWrite;
	unsigned int nRead;
	std::vector<T> vElems;
};
