#pragma once

#include <sys/epoll.h>
#include <vector>

class EPoller
{
public:
	EPoller();
	~EPoller();

	void AddEpoll(int socketfd);
	void DelEpoll(int socketfd);
	void EpollWait();

private:
	int fEpoll; // epoll文件描述符
	std::vector<struct epoll_event> mEvents;
};
