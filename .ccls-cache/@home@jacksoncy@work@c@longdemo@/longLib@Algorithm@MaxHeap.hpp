#pragma once
//最大堆

#include <algorithm>
#include <mutex>
#include <shared_mutex>
#include <vector>

template <typename T>
class MaxHeap
{
public:
	MaxHeap() : MaxHeap(30) {}
	//向量的预分配空间
	explicit MaxHeap(int capacity) { mArray.reserve(capacity); }
	//释放向量的空间
	~MaxHeap() { mArray.shrink_to_fit(); }

	bool Empty() { return mArray.empty(); }

	size_t Size() { return mArray.size(); }

	T &operator[](int i) { return mArray[i]; }

	bool Push(const T &elem)
	{
		std::unique_lock<std::shared_mutex> lock(mMutex);
		mArray.emplace_back(elem);
		up(mArray.size() - 1);
		return true;
	}

	//移除最顶堆元素
	T Pop()
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
		// return false;
	}

private:
	//上浮
	void up(int index)
	{
		int n, p;
		while (index > 0) {
			n = index % 2;
			if (n == 0) {
				p = (index - 2) / 2;
			} else {
				p = (index - 1) / 2;
			}
			if (mArray[index] > mArray[p]) {
				std::swap(mArray[index], mArray[p]);
				index = p;
			} else {
				break;
			}
		}
	}
	//下沉
	void down(int index)
	{
		int size = mArray.size();
		while (2 * index + 1 <= size - 1) {
			if (mArray[index] < mArray[2 * index + 1]) {
				std::swap(mArray[index], mArray[2 * index + 1]);
				if (2 * index + 2 <= size - 1 &&
					mArray[index] < mArray[2 * index + 2]) {
					std::swap(mArray[index], mArray[2 * index + 2]);
				}
				index = 2 * index + 1;
			} else {
				break;
			}
		}
	}

private:
	std::vector<T> mArray;
	mutable std::shared_mutex mMutex;
};
