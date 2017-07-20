#include "Timer.h"

Timer::Timer(std::chrono::microseconds tick) : nTick(tick) {}

Timer::~Timer() {}

unsigned int Timer::SetTimer(double interval) { return 0; }

void Timer::StopTimer(unsigned int timeId) {}

void Timer::timerExec() {}

void Timer::AsyncWait()
{
	while (true) {
		this->timerExec();
	}
}
