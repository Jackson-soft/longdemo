#pragma once

#include <vector>

template <typename T>
class BinaryHeap
{
public:
    BinaryHeap();
    ~BinaryHeap();

    bool isEmpty();
    void insert(const T& x);
    void deleteMin();
    void makeEmpty();


private:
    int currentSize;
    std::vector<T> array;

    void buidlHeap();
    void percolateDown(int hole);
};
