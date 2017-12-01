#include "Timer.h"

Timer::Timer(std::chrono::microseconds tick) : mTick(tick) {}

Timer::Timer() : Timer(std::chrono::microseconds(1)) {}

Timer::~Timer() {}

unsigned int Timer::SetOnceTimer(double interval, Action func) { return 0; }

unsigned int Timer::SetLoopTimer(double interval, Action fucn) { return 0; }

void Timer::StopTimer(unsigned int timeId) {}

void Timer::AsyncWait()
{
	while (true) {
		this->timerExec();
	}
}

void Timer::timerExec() {}
