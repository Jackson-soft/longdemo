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
	mh.Insert(3);
	mh.Insert(4);
	mh.Insert(8);
	mh.Insert(1);
	mh.Insert(9);
	for (int i = 0; i < mh.Size(); ++i) {
		std::cout << mh[i] << std::endl;
	}
	std::cout << "==========" << std::endl;
	MinHeap<int> minH;
	minH.Insert(3);
	minH.Insert(4);
	minH.Insert(8);
	minH.Insert(1);
	minH.Insert(9);
	for (int i = 0; i < minH.Size(); ++i) {
		std::cout << minH[i] << " ";
	}
	std::cout << std::endl;
	std::cout << minH.Remove() << std::endl;
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
