#pragma once

#include "utils/noncopyable.hpp"
#include <map>
#include <sys/poll.h>

namespace Uranus::Net
{
enum class EventType : unsigned int {
    Read  = POLLIN | POLLPRI,
    Write = POLLOUT,
};

//这个基类定义事件循环的接口
class EventLoop : public Utils::Noncopyable
{
public:
    EventLoop()          = default;
    virtual ~EventLoop() = 0;

    virtual auto AddEvent(int fd, EventType tp) -> bool = 0;
    virtual auto DelEvent(int fd, EventType tp) -> bool = 0;
    virtual auto ModEvent(int fd, EventType tp) -> bool = 0;
    virtual auto Loop() -> std::map<int, EventType>     = 0;
};
}  // namespace Uranus::Net
