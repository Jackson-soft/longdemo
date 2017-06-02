#pragma once

//最小堆
template<typename T>
class MiniHeap
{
public:
    MiniHeap()
    {

    }
    ~MiniHeap()
    {

    }

    //创建一个空堆
    void Build();

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
};
