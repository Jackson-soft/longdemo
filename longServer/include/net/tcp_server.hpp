#pragma once

#include "utils/util.hpp"
#include <atomic>
#include <string_view>
#include <thread>
#include <unistd.h>

namespace Uranus::Net
{
// Tcp 服务器
class TcpServer: public Utils::Noncopyable
{
public:
    TcpServer() = default;

    ~TcpServer() = default;

    TcpServer(unsigned int workNum = 0): mWorkers(workNum), mRunning(true)
    {
        if (mWorkers == 0) {
            mWorkers = std::thread::hardware_concurrency();
        }
    }

    // 主循环
    bool ListenTcp(unsigned short port, std::string_view ip = {""})
    {
        initServer();
        while (1) {
        }
    }

private:
    // 初始化master,worker进程
    bool initServer()
    {
        for (unsigned int i = 0; i < mWorkers; ++i) {
            auto pid = ::fork();
            switch (pid) {
            case -1:
                //错误处理
                break;
            case 0:
                //子进程
                workerRun();
                break;
            default:
                //父进程
                break;
            }
        }
        return true;
    }

    // worker
    void workerRun()
    {
        while (1) {
            /* code */
        }
    }

    unsigned short mPort;   // 端口
    std::string_view mIP;   // ip地址
    unsigned int mWorkers;  // 工作进程数量
    std::atomic_bool mRunning{true};
};
}  // namespace Uranus::Net
