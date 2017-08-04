#pragma once

//这个基类定义事件循环的接口
class EventLoop
{
private:
public:
    EventLoop() = default;
    virtual ~EventLoop(){}

    virtual void AddEvent(int socketfd) = 0;
    virtual void DelEvent(int socketfd) = 0;
    virtual void EventWait(int timeout = 0) = 0;
};
