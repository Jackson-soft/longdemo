#include "Timer.h"


Timer::Timer(std::chrono::microseconds tick) : nTick(tick)
{

}

Timer::~Timer()
{

}

unsigned int Timer::StartTimer(double interval)
{

}

void Timer::DeleteTimer(unsigned int timeId)
{

}

void Timer::TimerExec()
{

}


void Timer::AsyncWait()
{
    while (true) {
        this->TimerExec();
    }
}
