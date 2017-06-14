#pragma once

#include <algorithm>
#include <utility>
#include <vector>

//最小堆
template <typename T>
class MinHeap
{
public:
	MinHeap() : MinHeap(30) {}
	explicit MinHeap(int capacity) { mArray.reserve(capacity); }

	~MinHeap() { mArray.shrink_to_fit(); }

	//插入新元素
	bool Insert(const T &elem);

	//获取堆顶元素
	T Remove();

	bool Empty() { return mArray.empty(); }

	size_t Size() { return mArray.size(); }

	T &operator[](int i) { return mArray[i]; }

private:
	std::vector<T> mArray;
};

template <typename T>
bool MinHeap<T>::Insert(const T &elem)
{
	mArray.emplace_back(elem);
	int nCurrent = mArray.size() - 1;
	while (nCurrent != 0 && elem < mArray[(nCurrent - 1) / 2]) {
		std::swap(mArray[nCurrent], mArray[(nCurrent - 1) / 2]);
		nCurrent = (nCurrent - 1) / 2;
	}
	return true;
}

template <typename T>
T MinHeap<T>::Remove()
{
	if (!mArray.empty()) {
		//出堆的数据
		T mMin = mArray.front();
		//最后一个数据放到第一个根上面
		std::swap(mArray.front(), mArray.back());
		mArray.pop_back();
		int nCurrent = 0;
		while (nCurrent < mArray.size() && 2 * nCurrent + 2 < mArray.size()) {
			if (mArray[nCurrent] > mArray[2 * nCurrent + 1]) {
				std::swap(mArray[nCurrent], mArray[2 * nCurrent + 1]);
				nCurrent = 2 * nCurrent + 1;
				if (mArray[nCurrent] > mArray[nCurrent + 1]) {
					std::swap(mArray[nCurrent], mArray[nCurrent + 1]);
					nCurrent += 1;
				}
			}
		}
		return std::move(mMin);
	}
	//这里有一个警告，目前还没想到怎么去除
	// return xxx;
}
