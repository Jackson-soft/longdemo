#pragma once

#include "epoller.hpp"
#include "event_loop.hpp"
#include "listener.hpp"
#include "utils/util.hpp"
#include <atomic>
#include <cstdint>
#include <memory>
#include <string_view>
#include <thread>
#include <type_traits>
#include <unistd.h>

namespace Uranus
{
namespace Net
{
// Tcp 服务器
class TcpServer : public Utils::Noncopyable
{
public:
    TcpServer() : TcpServer(std::thread::hardware_concurrency()) {}

    ~TcpServer() { listener->Shutdown(); };

    TcpServer(const std::uint32_t number = 0) : workers(number)
    {
        if (workers == 0) {
            workers = std::thread::hardware_concurrency();
        }
    }

    // 监听端口
    bool Listen(const std::uint16_t port, std::string_view ip = {""})
    {
        // 创建listener并开始监听
        listener = std::make_unique<TcpListener>();
        return false;
    }

    // 是否成为守护进程
    bool Daemon()
    {
        auto pid = ::fork();
        switch (pid) {
        case -1:
            //错误处理
            return false;
        case 0:
            //子进程
            break;
        default:
            //父进程
            break;
        }
        return true;
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
    std::atomic_bool mRunning{true};
};
}  // namespace Net
}  // namespace Uranus
