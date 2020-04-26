#pragma once

#include "utils/util.hpp"
#include <sys/poll.h>

namespace Uranus
{
namespace Net
{
enum class EventType : unsigned int {
    Read  = POLLIN | POLLPRI,
    Write = POLLOUT,
};

//这个基类定义事件循环的接口
class EventLoop : public Noncopyable
{
public:
    EventLoop()          = default;
    virtual ~EventLoop() = 0;

    virtual int AddEvent(int fd, EventType tp) = 0;
    virtual int DelEvent(int fd, EventType tp) = 0;
    virtual int ModEvent(int fd, EventType tp) = 0;
    virtual int Run()                          = 0;
};
}  // namespace Net
}  // namespace Uranus
