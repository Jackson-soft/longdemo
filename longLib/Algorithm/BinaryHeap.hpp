#pragma once
//二叉堆
#include <vector>

template <typename T>
class BinaryHeap
{
public:
	BinaryHeap();
	explicit BinaryHeap(int nSize);
	~BinaryHeap();

	bool Empty() { return mArray.empty(); }
	void Push(const T &x);
	void DeleteMin();
	void MakeEmpty();

private:
	std::vector<T> mArray;
};
