#pragma once

#include "noncopyable.hpp"

namespace uranus::utils {
//这个基类定义事件循环的接口
class EventLoop : public Noncopyable
{
public:
    EventLoop()                          = default;
    virtual ~EventLoop()                 = 0;

    virtual auto addEvent(int fd) -> int = 0;
    virtual auto delEvent(int fd) -> int = 0;
    virtual auto modEvent(int fd) -> int = 0;
    virtual auto run() -> int            = 0;
};
}  // namespace uranus::utils
