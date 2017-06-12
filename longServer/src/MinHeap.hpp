#pragma once

#include <vector>

//最小堆
template<typename T>
class MinHeap
{
public:
    explicit MinHeap(int heapSize);

    ~MinHeap();

    //插入新元素
    void Insert(const T& elem);

    //将新元素提升使其匹配堆的性质
    void Update();

    //获取当前堆顶元素的值
    void Get();

    //删除堆顶元素
    void Delete();

    //使删除堆顶元素的堆再次成为堆
    void heapify();
private:
    int currentSize, maxSize;
    std::vector<T> heap;
};

template<typename T>
MinHeap<T>::MinHeap(int heapSize):currentSize(0), maxSize(heapSize)
{
}

template<typename T>
MinHeap<T>::~MinHeap()
{
    delete []heap;
}
