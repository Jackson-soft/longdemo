#pragma once

#include <vector>

template <typename T>
class BinaryHeap
{
public:
	BinaryHeap();
	explicit BinaryHeap(int nSize);
	~BinaryHeap();

	bool Empty() { return mArray.empty(); }
	void Insert(const T &x);
	void DeleteMin();
	void MakeEmpty();

private:
	std::vector<T> mArray;
};
