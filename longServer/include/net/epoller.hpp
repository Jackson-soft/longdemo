#pragma once

#include "event_loop.hpp"
#include <atomic>
#include <bits/stdint-uintn.h>
#include <cassert>
#include <cstring>
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>

namespace Uranus::Net
{
// Reactor 模式
// 消息循环的epoll实现
class Epoller : public EventLoop
{
public:
    // 委托构造
    Epoller() : Epoller(0) {}

    Epoller(int timeout) : epoll(::epoll_create1(EPOLL_CLOEXEC)), timeout(timeout)
    {
        assert(epoll != -1);
        events.reserve(16);
    }

    ~Epoller() override
    {
        events.clear();
        ::close(epoll);
    }

    int AddEvent(int fd, EventType tp) override
    {
        struct epoll_event event {
        };
        std::memset(&event, 0, sizeof(event));
        event.data.fd = fd;
        event.events  = static_cast<uint32_t>(tp);
        return ::epoll_ctl(epoll, EPOLL_CTL_ADD, fd, &event);
    }

    int DelEvent(int fd, EventType tp) override
    {
        struct epoll_event event {
        };
        std::memset(&event, 0, sizeof(event));
        event.data.fd = fd;
        event.events  = static_cast<uint32_t>(tp);
        return ::epoll_ctl(epoll, EPOLL_CTL_DEL, fd, &event);
    }

    int ModEvent(int fd, EventType tp) override
    {
        struct epoll_event event {
        };
        std::memset(&event, 0, sizeof(event));
        event.data.fd = fd;
        event.events  = static_cast<uint32_t>(tp);
        return ::epoll_ctl(epoll, EPOLL_CTL_MOD, fd, &event);
    }

    auto Run() -> int override
    {
        auto read{0};

        while (true) {
            // timeout：-1永久阻塞，0立即返回，非阻塞，>0指定微秒数
            read = ::epoll_wait(epoll, &*events.begin(), static_cast<int>(events.size()), timeout);
            if (read <= 0) {
                continue;
            }
            for (size_t i = 0; i < static_cast<size_t>(read); ++i) {
                if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!(events[i].events & EPOLLIN))) {
                    ::close(events[i].data.fd);
                    continue;
                } else if (events.at(i).data.fd == fd) {
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

private:
    // epoll文件描述符
    int epoll{0};

    int timeout{0};

    std::vector<struct epoll_event> events;
};
}  // namespace Uranus::Net
