#pragma once

#include "event_loop.hpp"
#include <atomic>
#include <bits/stdint-uintn.h>
#include <cassert>
#include <cstring>
#include <map>
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

    auto AddEvent(int fd, EventType tp) -> bool override
    {
        struct epoll_event event {
        };
        std::memset(&event, 0, sizeof(event));
        event.data.fd = fd;
        event.events  = static_cast<uint32_t>(tp);
        return ::epoll_ctl(epoll, EPOLL_CTL_ADD, fd, &event) == 0;
    }

    auto DelEvent(int fd, EventType tp) -> bool override
    {
        struct epoll_event event {
        };
        std::memset(&event, 0, sizeof(event));
        event.data.fd = fd;
        event.events  = static_cast<uint32_t>(tp);
        return ::epoll_ctl(epoll, EPOLL_CTL_DEL, fd, &event) == 0;
    }

    auto ModEvent(int fd, EventType tp) -> bool override
    {
        struct epoll_event event {
        };
        std::memset(&event, 0, sizeof(event));
        event.data.fd = fd;
        event.events  = static_cast<uint32_t>(tp);
        return ::epoll_ctl(epoll, EPOLL_CTL_MOD, fd, &event) == 0;
    }

    auto Loop() -> std::map<int, EventType> override
    {
        auto read{0};

        // timeout：-1永久阻塞，0立即返回，非阻塞，>0指定微秒数
        read = ::epoll_wait(epoll, &*events.begin(), static_cast<int>(events.size()), timeout);
        std::map<int, EventType> result{};
        if (read > 0) {
            for (size_t i = 0; i < static_cast<size_t>(read); ++i) {
                EventType et;
                if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!(events[i].events & EPOLLIN))) {
                    ::close(events[i].data.fd);
                    continue;
                }
                if (events.at(i).events & EPOLLIN) {
                    // read
                    et = EventType::Read;
                } else if (events.at(i).events & EPOLLOUT) {
                    // write
                    et = EventType::Write;
                }
                result.emplace(std::make_pair(events.at(i).data.fd, et));
            }
        }
        return result;
    }

private:
    // epoll文件描述符
    int epoll{0};

    int timeout{0};

    std::vector<struct epoll_event> events;
};
}  // namespace Uranus::Net
