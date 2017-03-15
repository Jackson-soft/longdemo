#pragma once


#include <thread>
#include <iostream>

class ThreadPool
{
private:
    ~ThreadPool();
    int nThreadNum;

public:
    ThreadPool();
    void GetNum();
};
