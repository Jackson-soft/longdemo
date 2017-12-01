#pragma once

//这个基类定义事件循环的接口
class EventLoop
{
public:
    EventLoop() = default;
    virtual ~EventLoop() {}

    virtual void AddEvent(int eventfd) = 0;
    virtual void DelEvent(int eventfd) = 0;
    virtual void ModEvent(int eventfd) = 0;
    virtual void EventWait(int eventfd, int timeout = 0) = 0;

private:
};
