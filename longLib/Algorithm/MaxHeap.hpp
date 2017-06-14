#pragma once
#include <algorithm>
#include <vector>

template <typename T>
class MaxHeap
{
private:
	std::vector<T> mArray;

public:
	MaxHeap() : MaxHeap(30) {}

	//向量的预分配空间
	explicit MaxHeap(int capacity) { mArray.reserve(capacity); }

	//释放向量的空间
	~MaxHeap() { mArray.shrink_to_fit(); }

	bool Empty() { return mArray.empty(); }

	size_t Size() { return mArray.size(); }

	T &operator[](int i) { return mArray[i]; }

	bool Insert(const T &elem);

	//移除最顶堆元素
	bool Remove();
};

template <typename T>
bool MaxHeap<T>::Insert(const T &elem)
{
	mArray.emplace_back(elem);
	//保存添加元素前的向量容量
	auto nCurrent = mArray.size() - 1;
	while (nCurrent != 0 && elem > mArray[(nCurrent - 1) / 2]) {
		std::swap(mArray[nCurrent], mArray[(nCurrent - 1) / 2]);
		nCurrent = (nCurrent - 1) / 2;
	}

	return true;
}

template <typename T>
bool MaxHeap<T>::Remove()
{
	if (!mArray.empty()) {
		//出堆的数据
		// T mMin = mArray.front();
		//最后一个数据放到第一个根上面
		std::swap(mArray.front(), mArray.back());
		mArray.pop_back();
		int nCurrent = 0;
		while (nCurrent < mArray.size() && 2 * nCurrent + 2 < mArray.size()) {
			if (mArray[nCurrent] < mArray[2 * nCurrent + 1]) {
				std::swap(mArray[nCurrent], mArray[2 * nCurrent + 1]);
				nCurrent = 2 * nCurrent + 1;
				if (mArray[nCurrent] < mArray[nCurrent + 1]) {
					std::swap(mArray[nCurrent], mArray[nCurrent + 1]);
					nCurrent += 1;
				}
			}
		}
		return true;
	}
	return false;
}
