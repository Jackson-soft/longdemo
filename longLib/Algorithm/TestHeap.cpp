#include <iostream>
#include <utility>
#include <vector>

#include "../../longServer/src/MinHeap.hpp"
#include "MaxHeap.hpp"

template <typename T>
T re(T x, T y)
{
	return std::move(x + y);
}

int main()
{
	MaxHeap<int> mh;
	mh.Push(3);
	mh.Push(4);
	mh.Push(8);
	mh.Push(1);
	mh.Push(9);
	for (int i = 0; i < mh.Size(); ++i) {
		std::cout << mh[i] << " ";
	}
	std::cout << std::endl;
	std::cout << "==========" << std::endl;
	std::cout << mh.Pop() << std::endl;
	for (int i = 0; i < mh.Size(); ++i) {
		std::cout << mh[i] << " ";
	}
	std::cout << std::endl;
	std::cout << "==========" << std::endl;
	MinHeap<int> minH;
	minH.Push(3);
	minH.Push(4);
	minH.Push(8);
	minH.Push(1);
	minH.Push(9);
	for (int i = 0; i < minH.Size(); ++i) {
		std::cout << minH[i] << " ";
	}
	std::cout << std::endl;
	std::cout << minH.Pop() << std::endl;
	for (int i = 0; i < minH.Size(); ++i) {
		std::cout << minH[i] << " ";
	}
	std::cout << std::endl;
	std::cout << "---" << std::endl;
	std::cout << re(3, 4) << std::endl;
	std::vector<int> A{3, 6, 5};
	A.pop_back();
	for (auto &it : A) {
		std::cout << it << std::endl;
	}
	std::cout << A.front() << "," << A.size() << std::endl;
	return 0;
}
