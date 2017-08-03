#pragma once

class EventLoop
{
private:
public:
    EventLoop();
    ~EventLoop();

    void AddEvent();
    void DelEvent();
};
