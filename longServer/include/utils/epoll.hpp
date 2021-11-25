#pragma once

#include "event_loop.hpp"

#include <atomic>
#include <cassert>
#include <cstring>
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>

namespace uranus::utils {
// Reactor 模式
// 消息循环的epoll实现
class Epoller : public EventLoop
{
public:
    // 委托构造
    Epoller() : Epoller(0) {}

    Epoller(int timeout_) : epoll_(::epoll_create1(EPOLL_CLOEXEC)), timeout_(timeout_) {
        assert(epoll_ != -1);
        events_.reserve(16);
    }

    ~Epoller() override {
        if (running_.load()) {
            running_.store(false, std::memory_order_release);
        }
        ::close(epoll_);
    }

    auto addEvent(int fd) -> int override {
        struct epoll_event event {};
        std::memset(&event, 0, sizeof(event));
        event.data.fd = fd;
        event.events  = EPOLLIN | EPOLLET;
        return ::epoll_ctl(epoll_, EPOLL_CTL_ADD, fd, &event);
    }

    auto delEvent(int fd) -> int override {
        struct epoll_event event {};
        std::memset(&event, 0, sizeof(event));
        event.data.fd = fd;
        return ::epoll_ctl(epoll_, EPOLL_CTL_DEL, fd, &event);
    }

    auto modEvent(int fd) -> int override {
        struct epoll_event event {};
        std::memset(&event, 0, sizeof(event));
        event.data.fd = fd;
        return ::epoll_ctl(epoll_, EPOLL_CTL_MOD, fd, &event);
    }

    auto run() -> int override {
        int nReady{0};

        while (running_.load()) {
            // timeout：-1永久阻塞，0立即返回，非阻塞，>0指定微秒数
            nReady = ::epoll_wait(epoll_, &*events_.begin(), static_cast<int>(events_.size()), timeout_);
            if (nReady <= 0) {
                continue;
            }
            for (size_t i = 0; i < static_cast<size_t>(nReady); ++i) {
                if ((events_[i].events & EPOLLERR) || (events_[i].events_ & EPOLLHUP)
                    || (!(events_[i].events & EPOLLIN))) {
                    ::close(events_[i].data.fd);
                    continue;
                    //} else if (events_.at(i).data.fd == fd) {
                    // accept
                } else if (events_.at(i).events & EPOLLIN) {
                    // read
                } else if (events_.at(i).events & EPOLLOUT) {
                    // write
                }
            }
        }
        return 0;
    }

    void stop() {
        running_.store(false, std::memory_order_release);
    }

private:
    // epoll文件描述符
    int                             epoll_{0};
    int                             timeout_{0};
    std::vector<struct epoll_event> events_;
    std::atomic_bool                running_{true};
};
}  // namespace uranus::utils
