#pragma once
#include <algorithm>
#include <vector>

template <typename T>
class MaxHeap
{
private:
	std::vector<T> mArray;

public:
	MaxHeap();
	explicit MaxHeap(int capacity);
	~MaxHeap();

	bool Insert(const T &elem);
	bool Remove();

	bool Empty() { return mArray.empty(); }

	size_t Size() { return mArray.size(); }

	T &operator[](int i) { return mArray[i]; }
};

template <typename T>
MaxHeap<T>::MaxHeap() : MaxHeap(10)
{
}

template <typename T>
MaxHeap<T>::MaxHeap(int capacity)
{
	mArray.reserve(capacity); //给vector预分配存储空间
}

template <typename T>
MaxHeap<T>::~MaxHeap()
{
	mArray.shrink_to_fit(); //释放vector内存
}

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

	return true;
}
