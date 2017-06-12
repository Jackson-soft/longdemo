#pragma once
#include <vector>

template <typename T>
class MaxHeap
{
private:
	std::vector<T> mArray;
	int nCapacity; //总容量
	int nSize;	 //实际大小

private:
	void filterDown();
	void filterUp(int start);

public:
	MaxHeap();
	MaxHeap(int capacity);
	~MaxHeap();

	int insert(T data);
	void remove(int index);
};

template <typename T>
MaxHeap<T>::MaxHeap() : MaxHeap(10)
{
}

template <typename T>
MaxHeap<T>::MaxHeap(int capacity) : nCapacity(capacity), nSize(0)
{
	mArray.reserve(nCapacity); //给vector预分配存储空间
}

template <typename T>
MaxHeap<T>::~MaxHeap()
{
	nSize	 = 0;
	nCapacity = 0;
	mArray.shrink_to_fit(); //释放vector内存
}

template <typename T>
void MaxHeap<T>::filterUp(int start)
{
}

template <typename T>
void MaxHeap<T>::filterDown()
{
}
