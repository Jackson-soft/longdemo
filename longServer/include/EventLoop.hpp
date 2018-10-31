#pragma once

#include "Util.hpp"

//这个基类定义事件循环的接口
class EventLoop : Noncopyable
{
public:
	EventLoop()			 = default;
	virtual ~EventLoop() = 0;

	virtual int AddEvent(int fd) = 0;
	virtual int DelEvent(int fd) = 0;
	virtual int ModEvent(int fd) = 0;
	virtual int Run()			 = 0;
};
