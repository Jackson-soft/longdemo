#pragma once

#include "net/socket.hpp"
#include "utils/noncopyable.hpp"

#include <chrono>
#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <unistd.h>

// 连接器
namespace uranus::net {
// 连接器类
class Dialer : utils::Noncopyable, std::enable_shared_from_this<Dialer> {
public:
    Dialer() = default;
    ~Dialer() {
        Close();
    }

    //连接到网络地址
    auto Dial(const std::string_view network, const std::string_view ip, const unsigned short port) -> bool {
        if (network.empty() || ip.empty() || port <= 0) {
            return false;
        }
        remoteAddr_ = ip;
        port_       = port;

        if (!socket_.NewSocket(network)) {
            return false;
        }

        return socket_.Connect(ip, port);
    }

    auto Read() -> int {
        return 0;
    }

    auto Write() -> int {
        return 0;
    }

    auto SetKeepAlive(bool on) -> bool {
        return socket_.SetKeeplive(on);
    }

    void Close() {
        socket_.Close();
    }

    auto Shutdown() -> bool {
        return socket_.ShutDown();
    }

private:
    // Socket对象
    net::Socket                socket_;
    std::chrono::duration<int> timeout_;
    //客户端地址
    std::string                localAddr_;
    //远程地址
    std::string                remoteAddr_;
    unsigned short             port_{0};
    std::chrono::duration<int> keepAlive_;
};
}  // namespace uranus::net
