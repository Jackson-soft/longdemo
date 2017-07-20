#pragma once
//最小堆
#include <algorithm>
#include <mutex>
#include <shared_mutex>
#include <utility>
#include <vector>

template <typename T>
class MinHeap
{
public:
	MinHeap() : MinHeap(30) {}
	//这是做一下预分配内存
	explicit MinHeap(int capacity) { mArray.reserve(capacity); }

	~MinHeap() { mArray.shrink_to_fit(); }

	//插入新元素
	bool Push(const T &elem);

	//获取堆顶元素
	T Pop();

	bool Empty() { return mArray.empty(); }

	size_t Size() { return mArray.size(); }

	T &operator[](int i) { return mArray[i]; }

private:
	//上浮
	void up(int index);
	//下沉
	void down(int index);

private:
	std::vector<T> mArray;
	// mutable声明可变数据成员
	mutable std::shared_mutex mMutex;
};

template <typename T>
bool MinHeap<T>::Push(const T &elem)
{
	//独享写锁
	std::unique_lock<std::shared_mutex> lock(mMutex);
	mArray.emplace_back(elem);
	up(mArray.size() - 1);
	return true;
}

template <typename T>
void MinHeap<T>::up(int index)
{
	int n, p;
	while (index > 0) {
		n = index % 2;
		if (n == 0) {
			p = (index - 2) / 2;
		} else {
			p = (index - 1) / 2;
		}
		if (mArray[index] < mArray[p]) {
			std::swap(mArray[index], mArray[p]);
			index = p;
		} else {
			break;
		}
	}
}

template <typename T>
T MinHeap<T>::Pop()
{
	//共享读锁
	std::shared_lock<std::shared_mutex> lock(mMutex);
	if (!mArray.empty()) {
		//出堆的数据
		T mMin = mArray.front();
		//最后一个数据放到第一个根上面
		std::swap(mArray.front(), mArray.back());
		mArray.pop_back();
		down(0);
		return std::move(mMin);
	}
	//这里有一个警告，目前还没想到怎么去除
	// return xxx;
}

template <typename T>
void MinHeap<T>::down(int index)
{
	int size = mArray.size();
	while (2 * index + 1 <= size - 1) {
		if (mArray[index] > mArray[2 * index + 1]) {
			std::swap(mArray[index], mArray[2 * index + 1]);
			if (2 * index + 2 <= size - 1 &&
				mArray[index] > mArray[2 * index + 2]) {
				std::swap(mArray[index], mArray[2 * index + 2]);
			}
			index = 2 * index + 1;
		} else {
			break;
		}
	}
}
