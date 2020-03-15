#pragma once

#include "utils/min_heap.hpp"
#include "utils/util.hpp"
#include <chrono>
#include <cstdint>
#include <functional>

namespace Uranus
{
//定时器
class Timer : Utils::Noncopyable
{
public:
    using Action = std::function<void()>;
    static Timer &Get()
    {
        static Timer timer;
        return timer;
    }

    // 单次
    std::uint32_t OnceTimer(double interval, Action func) { return 0; }

    // 循环
    std::uint32_t LoopTimer(double interval, Action fucn) { return 0; }

    //取消定时器
    void Stop(std::uint32_t id) {}

    //异步等待
    void AsyncWait()
    {
        while (true) {
            this->exec();
        }
    }

private:
    Timer() {}

    Timer(std::chrono::duration<int> tick) : mTick(tick) {}

    ~Timer() {}

    //定时器循环
    void exec() {}

private:
    std::chrono::duration<int> mTick{1};  //定时器的粒度
};
}  // namespace Uranus