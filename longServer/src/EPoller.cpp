#include "EPoller.h"
#include <unistd.h>

EPoller::EPoller()
{
	mEpoll = epoll_create1(EPOLL_CLOEXEC);
	if (mEpoll == -1) {
		//
	}
    mEvents.reserve(16);
}

EPoller::~EPoller() { ::close(mEpoll); }

void EPoller::AddEvent(int socketfd)
{
	struct epoll_event event;
	::epoll_ctl(mEpoll, EPOLL_CTL_ADD, socketfd, &event);
}

void EPoller::DelEvent(int socketfd)
{
	struct epoll_event event;
	::epoll_ctl(mEpoll, EPOLL_CTL_DEL, socketfd, &event);
}

void EPoller::ModEvent(int socketfd)
{
	struct epoll_event event;
	::epoll_ctl(mEpoll, EPOLL_CTL_MOD, socketfd, &event);
}

void EPoller::EventWait(int eventfd, int timeout)
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
