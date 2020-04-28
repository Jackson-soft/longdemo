#pragma once

#include "epoller.hpp"
#include "event_loop.hpp"
#include "listener.hpp"
#include "utils/noncopyable.hpp"
#include "utils/thread_pool.hpp"
#include <atomic>
#include <bits/types/sigset_t.h>
#include <csignal>
#include <cstdint>
#include <memory>
#include <queue>
#include <string_view>
#include <thread>
#include <type_traits>
#include <unistd.h>

namespace Uranus::Net
{
// Tcp 服务器
class TcpServer : public Utils::Noncopyable
{
public:
    TcpServer() : TcpServer(std::thread::hardware_concurrency()) {}

    ~TcpServer() { listener->Close(); };

    TcpServer(const std::uint32_t number = 0) : workers(number == 0 ? std::thread::hardware_concurrency() : number) {}

    // 监听端口
    auto Listen(const std::uint16_t port, std::string_view ip = {""}) -> bool
    {
        // 创建listener并开始监听
        listener = std::make_unique<TcpListener>();
        return false;
    }

    // 是否成为守护进程
    auto Daemon() -> bool
    {
        // glibc > 2.21
        return ::daemon(0, 0) == 0;
    }

    // Ctrl-C 信号
    auto Signal()
    {
        sigset_t set;
        if (::sigemptyset(&set) < 0)
            return false;
        if (::sigaddset(&set, SIGINT) < 0)
            return false;
        return ::sigprocmask(SIG_SETMASK, &set, nullptr) == 0;
    }

    // 主循环
    void Run()
    {
        while (true) {
        }
    }

private:
    std::uint32_t workers;  // 工作进程数量
    std::unique_ptr<TcpListener> listener;
    std::unique_ptr<EventLoop> loop;
    std::queue<Utils::ThreadPool> worker;
    std::atomic_bool running{true};
};
}  // namespace Uranus::Net
