#pragma once

template <typename T>
class MaxHeap
{
private:
	T *mHeap;
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
MaxHeap<T>::MaxHeap()
{
}

template <typename T>
MaxHeap<T>::MaxHeap(int capacity) : nCapacity(capacity)
{
	nSize = 0;
	mHeap = new T[nCapacity];
}

template <typename T>
MaxHeap<T>::~MaxHeap() : MaxHeap(10)
{
	nSize	 = 0;
	nCapacity = 0;
	delete[] mHeap;
}

template <typename T>
void MaxHeap<T>::filterUp(int start)
{
}

template <typename T>
void MaxHeap<T>::filterDown()
{
}
