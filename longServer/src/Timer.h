#pragma once

#include <chrono>

//定时器
class Timer
{
private:
    std::chrono::microseconds nTick; //定时器的粒度

    Timer(std::chrono::microseconds tick);

    //定时器循环
    void TimerExec();
public:
    ~Timer();

    static Timer* GetInstance(std::chrono::microseconds tick) {
        static Timer timer(tick);
        return &timer;
    }

    //开始
    unsigned int StartTimer(double interval);

    //取消定时器
    void DeleteTimer(unsigned int timeId);

    //异步等待
    void AsyncWait();
};
