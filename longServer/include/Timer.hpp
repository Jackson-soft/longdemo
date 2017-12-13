#pragma once

#include "MinHeap.hpp"
#include "Util.hpp"
#include <chrono>
#include <functional>

//定时器
class Timer : Noncopyable
{
	using Action = std::function<void()>;

public:
	static Timer *GetInstance()
	{
		static Timer timer;
		return &timer;
	}

	// 单次
	unsigned int SetOnceTimer(double interval, Action func) { return 0; }

	// 循环
	unsigned int SetLoopTimer(double interval, Action fucn) { return 0; }

	//取消定时器
	void StopTimer(unsigned int timeId) {}

	//异步等待
	void AsyncWait()
	{
		while (true) {
			this->timerExec();
		}
	}

private:
	std::chrono::microseconds mTick; //定时器的粒度

	Timer(std::chrono::microseconds tick) : mTick(tick) {}

	Timer() {}

	~Timer() {}

	//定时器循环
	void timerExec() {}
};
