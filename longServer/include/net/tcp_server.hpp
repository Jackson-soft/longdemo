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
#include <sys/socket.h>
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

    TcpServer(const std::uint32_t number = 0) : workers(number == 0 ? std::thread::hardware_concurrency() : number)
    {
        worker = std::make_unique<Utils::ThreadPool>(workers);
        // 创建listener
        listener = std::make_unique<TcpListener>();
        loop     = std::make_unique<Epoller>();
    }

    // 监听端口
    auto Listen(const std::uint16_t port, std::string_view ip = {""}) -> bool
    {
        if (port <= 0)
            return false;

        if (!listener->Listen(port, ip))
            return false;

        return loop->AddEvent(listener->Native(), EventType::Read);
    }

    auto Listen(std::string_view addr) -> bool
    {
        if (addr.empty())
            return false;
        return false;
    }

    // 是否成为守护进程
    auto Daemon() -> bool
    {
        // glibc > 2.21
        return ::daemon(0, 0) == 0;
    }

    // Ctrl-C 信号
    auto Signal() -> bool
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
            auto eventMap = loop->Loop();
            for (const auto &it : eventMap) {
                if (it.first == listener->Native()) {
                    auto fd = listener->Accept();
                    loop->AddEvent(fd, EventType::Read);
                }

                if (it.second == EventType::Read) {
                }
            }
        }
    }

private:
    std::uint32_t workers;  // 工作进程数量
    std::unique_ptr<TcpListener> listener;
    std::unique_ptr<EventLoop> loop;
    std::unique_ptr<Utils::ThreadPool> worker;
    std::atomic_bool running{true};
};
}  // namespace Uranus::Net
