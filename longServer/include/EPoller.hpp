#pragma once

#include "EventLoop.hpp"
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>

// 消息循环的epoll实现
class EPoller : public EventLoop
{
public:
	EPoller()
	{
		mEpoll = epoll_create1(EPOLL_CLOEXEC);
		if (mEpoll == -1) {
			//
		}
		mEvents.reserve(16);
	}
	~EPoller() { ::close(mEpoll); }

	void AddEvent(int socketfd) override
	{
		struct epoll_event event;
		::epoll_ctl(mEpoll, EPOLL_CTL_ADD, socketfd, &event);
	}

	void DelEvent(int socketfd) override
	{
		struct epoll_event event;
		::epoll_ctl(mEpoll, EPOLL_CTL_DEL, socketfd, &event);
	}

	void ModEvent(int socketfd) override
	{
		struct epoll_event event;
		::epoll_ctl(mEpoll, EPOLL_CTL_MOD, socketfd, &event);
	}

	void EventWait(int eventfd, int timeout = 0) override
	{
		int nReady{0};

		while (true) {
			// timeout：-1永久阻塞，0立即返回，非阻塞，>0指定微秒数
			nReady = ::epoll_wait(mEpoll,
								  &*mEvents.begin(),
								  static_cast<int>(mEvents.size()),
								  timeout);
			if (nReady <= 0) {
				//这里做点啥呢
				continue;
			}
			for (size_t i = 0; i < static_cast<size_t>(nReady); ++i) {
				if (mEvents.at(i).data.fd == eventfd) {
					// accept
				} else if (mEvents.at(i).events & EPOLLIN) {
					// read
				} else if (mEvents.at(i).events & EPOLLOUT) {
					// write
				}
			}
		}
	}

private:
	int mEpoll; // epoll文件描述符
	std::vector<struct epoll_event> mEvents;
};
