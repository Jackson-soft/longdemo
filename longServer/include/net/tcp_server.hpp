#pragma once

#include "utils/noncopyable.hpp"

#include <atomic>
#include <string_view>
#include <thread>
#include <unistd.h>

namespace uranus::net {
// Tcp 服务器
class TcpServer : public utils::Noncopyable {
public:
    TcpServer()  = default;
    ~TcpServer() = default;

    explicit TcpServer(unsigned int workNum = 0) : workers_(workNum), running_(true) {
        if (workers_ == 0) {
            workers_ = std::thread::hardware_concurrency();
        }
    }

    // 主循环
    auto Listen(unsigned short port, std::string_view ip = {""}) -> bool {
        initServer();
        while (true) {}
    }

private:
    // 初始化master,worker进程
    auto initServer() -> bool {
        for (unsigned int i = 0; i < workers_; ++i) {
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
    void workerRun() {
        while (true) {
            /* code */
        }
    }

    unsigned short   port_{};   // 端口
    std::string_view ip_{};     // ip地址
    unsigned int     workers_;  // 工作进程数量
    std::atomic_bool running_{true};
};
}  // namespace uranus::net
