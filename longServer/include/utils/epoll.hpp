#pragma once

#include "event_loop.hpp"
#include <atomic>
#include <cassert>
#include <cstring>
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>

namespace uranus::utils
{
// Reactor 模式
// 消息循环的epoll实现
class Epoller: public EventLoop
{
public:
    // 委托构造
    Epoller(): Epoller(0) {}

    Epoller(int timeout): timeout(timeout), epoll(::epoll_create1(EPOLL_CLOEXEC);)
    {
        assert(epoll != -1);
        events.reserve(16);
    }

    ~Epoller() override
    {
        if (running.load()) {
            running.store(false, std::memory_order_release);
        }
        ::close(epoll);
    }

    int AddEvent(int fd) override
    {
        struct epoll_event event;
        std::memset(&event, 0, sizeof(event));
        event.data.fd = fd;
        event.events  = EPOLLIN | EPOLLET;
        return ::epoll_ctl(epoll, EPOLL_CTL_ADD, fd, &event);
    }

    int DelEvent(int fd) override
    {
        struct epoll_event event;
        std::memset(&event, 0, sizeof(event));
        event.data.fd = fd;
        return ::epoll_ctl(epoll, EPOLL_CTL_DEL, fd, &event);
    }

    int ModEvent(int fd) override
    {
        struct epoll_event event;
        std::memset(&event, 0, sizeof(event));
        event.data.fd = fd;
        return ::epoll_ctl(epoll, EPOLL_CTL_MOD, fd, &event);
    }

    int Run() override
    {
        int nReady{0};

        while (running.load()) {
            // timeout：-1永久阻塞，0立即返回，非阻塞，>0指定微秒数
            nReady = ::epoll_wait(epoll, &*events.begin(), static_cast<int>(events.size()), timeout);
            if (nReady <= 0) {
                continue;
            }
            for (size_t i = 0; i < static_cast<size_t>(nReady); ++i) {
                if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!(events[i].events & EPOLLIN))) {
                    ::close(events[i].data.fd);
                    continue;
                    //} else if (events.at(i).data.fd == fd) {
                    // accept
                } else if (events.at(i).events & EPOLLIN) {
                    // read
                } else if (events.at(i).events & EPOLLOUT) {
                    // write
                }
            }
        }
        return 0;
    }

    void Stop() { running.store(false, std::memory_order_release); }

private:
    // epoll文件描述符
    int epoll{0};

    int timeout{0};

    std::vector<struct epoll_event> events;

    std::atomic_bool running{true};
};
}  // namespace uranus::utils