#pragma once

#include "utils/min_heap.hpp"
#include "utils/util.hpp"
#include <chrono>
#include <cstdint>
#include <functional>

namespace Uranus::Utils
{
//定时器
class Timer: Noncopyable
{
public:
    using Action = std::function<void()>;

    static auto Get() -> Timer &
    {
        static Timer timer;
        return timer;
    }

    // 单次
    auto OnceTimer(double interval, Action func) -> std::uint32_t { return 0; }

    // 循环
    auto LoopTimer(double interval, Action fucn) -> std::uint32_t { return 0; }

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
    Timer() = default;

    Timer(std::chrono::duration<int> tick): mTick(tick) {}

    ~Timer() = default;

    //定时器循环
    void exec() {}

    std::chrono::duration<int> mTick{1};  //定时器的粒度
};
}  // namespace Uranus::Utils