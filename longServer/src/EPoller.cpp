#include "EPoller.h"
#include <unistd.h>

EPoller::EPoller()
{
	fEpoll = epoll_create1(EPOLL_CLOEXEC);
	if (fEpoll == -1) {
		//
	}
}

EPoller::~EPoller() { ::close(fEpoll); }

void EPoller::AddEvent(int socketfd)
{
    struct epoll_event event;
    ::epoll_ctl(fEpoll, EPOLL_CTL_ADD, socketfd, &event);
}

void EPoller::DelEvent(int socketfd)
{
    struct epoll_event event;
    ::epoll_ctl(fEpoll, EPOLL_CTL_DEL, socketfd, &event);
}

void EPoller::EventWait(int timeout)
{
	while (true) {
		// timeout：-1永久阻塞，0立即返回，非阻塞，>0指定微秒数
        int activeCnt = ::epoll_wait(fEpoll,
                                     &*mEvents.begin(),
                                     static_cast<int>(mEvents.size()),
                                     timeout);
		if (activeCnt <= 0) {
			//这里做点啥呢
            continue;
		}
		for (int i = 0; i < activeCnt; ++i) {
			// if(events[i].data.fd == listenFd)
            if (mEvents[i].events & EPOLLIN) {
				// read
            } else if (mEvents[i].events & EPOLLOUT) {
				// write
			}
		}
	}
}
