#include "ThreadPool.h"


ThreadPool::ThreadPool() : nThreadNum(2)
{
    nThreadNum = std::thread::hardware_concurrency();
}


ThreadPool::~ThreadPool()
{
    
}


void ThreadPool::GetNum()
{
    std::cout << nThreadNum << std::endl;
}
