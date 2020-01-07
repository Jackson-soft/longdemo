#pragma once

#include "event_loop.hpp"
#include <atomic>
#include <cassert>
#include <cstring>
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>

namespace Uranus
{
// Reactor 模式
// 消息循环的epoll实现
class Epoller : public EventLoop
{
public:
    // 委托构造
    Epoller() : Epoller(0) {}

    Epoller(int timeout) : mTimeout(timeout), mEpoll(::epoll_create1(EPOLL_CLOEXEC);)
    {
        assert(mEpoll != -1);
        mEvents.reserve(16);
    }

    ~Epoller() override
    {
        if (mRunning.load()) {
            mRunning.store(false, std::memory_order_release);
        }
        ::close(mEpoll);
    }

    int AddEvent(int fd) override
    {
        struct epoll_event event;
        std::memset(&event, 0, sizeof(event));
        event.data.fd = fd;
        event.events  = EPOLLIN | EPOLLET;
        return ::epoll_ctl(mEpoll, EPOLL_CTL_ADD, fd, &event);
    }

    int DelEvent(int fd) override
    {
        struct epoll_event event;
        std::memset(&event, 0, sizeof(event));
        event.data.fd = fd;
        return ::epoll_ctl(mEpoll, EPOLL_CTL_DEL, fd, &event);
    }

    int ModEvent(int fd) override
    {
        struct epoll_event event;
        std::memset(&event, 0, sizeof(event));
        event.data.fd = fd;
        return ::epoll_ctl(mEpoll, EPOLL_CTL_MOD, fd, &event);
    }

    int Run() override
    {
        int nReady{0};

        while (mRunning.load()) {
            // timeout：-1永久阻塞，0立即返回，非阻塞，>0指定微秒数
            nReady = ::epoll_wait(mEpoll, &*mEvents.begin(), static_cast<int>(mEvents.size()), mTimeout);
            if (nReady <= 0) {
                continue;
            }
            for (size_t i = 0; i < static_cast<size_t>(nReady); ++i) {
                if ((mEvents[i].events & EPOLLERR) || (mEvents[i].events & EPOLLHUP)
                    || (!(mEvents[i].events & EPOLLIN))) {
                    ::close(mEvents[i].data.fd);
                    continue;
                    //} else if (mEvents.at(i).data.fd == fd) {
                    // accept
                } else if (mEvents.at(i).events & EPOLLIN) {
                    // read
                } else if (mEvents.at(i).events & EPOLLOUT) {
                    // write
                }
            }
        }
        return 0;
    }

    void Stop() { mRunning.store(false, std::memory_order_release); }

private:
    // epoll文件描述符
    int mEpoll{0};

    int mTimeout{0};

    std::vector<struct epoll_event> mEvents;

    std::atomic_bool mRunning{true};
};
}  // namespace Uranus