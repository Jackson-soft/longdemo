#pragma once

#include "EventLoop.hpp"
#include <sys/epoll.h>
#include <vector>

// 消息循环的epoll实现
class EPoller : public EventLoop
{
public:
	EPoller();
	~EPoller();

    void AddEvent(int socketfd) override;
    void DelEvent(int socketfd) override;
    void EventWait(int timeout = 0) override;

private:
	int fEpoll; // epoll文件描述符
	std::vector<struct epoll_event> mEvents;
};
