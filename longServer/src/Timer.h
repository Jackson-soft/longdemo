#pragma once

#include "MinHeap.hpp"
#include "Util.hpp"
#include <chrono>
#include <functional>

//定时器
class Timer : noncopyable
{
	using Action = std::function<void()>;

public:
	static Timer *GetInstance()
	{
		static Timer timer;
		return &timer;
	}

	// 单次
	unsigned int SetOnceTimer(double interval, Action func);

	// 循环
	unsigned int SetLoopTimer(double interval, Action fucn);

	//取消定时器
	void StopTimer(unsigned int timeId);

	//异步等待
	void AsyncWait();

private:
	std::chrono::microseconds mTick; //定时器的粒度

	Timer(std::chrono::microseconds tick);

	Timer();

	~Timer();

	//定时器循环
	void timerExec();
};
