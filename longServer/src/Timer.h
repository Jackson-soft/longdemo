#pragma once

#include <chrono>

//定时器
class Timer
{
private:
    std::chrono::microseconds nTick; //定时器的粒度

    Timer(std::chrono::microseconds tick);

    void TimerExec();
public:
    ~Timer();

    static Timer* GetInstance(std::chrono::microseconds tick) {
        static Timer timer(tick);
        return &time;
    }

    unsigned int StartTimer(double interval);

    void DeleteTimer(unsigned int timeId);

    //异步等待
    void AsyncWait();
};
