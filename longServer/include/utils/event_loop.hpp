#pragma once

#include "util.hpp"

namespace uranus
{
//这个基类定义事件循环的接口
class EventLoop: public utils::Noncopyable
{
public:
    EventLoop()          = default;
    virtual ~EventLoop() = 0;

    virtual int AddEvent(int fd) = 0;
    virtual int DelEvent(int fd) = 0;
    virtual int ModEvent(int fd) = 0;
    virtual int Run()            = 0;
};
}  // namespace uranus