#pragma once

#include "net/tcp_socket.hpp"

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <unistd.h>

// 连接器
namespace uranus::net {
// 连接器类
class UnixDialer : public std::enable_shared_from_this<UnixDialer> {
public:
    UnixDialer() = default;

    ~UnixDialer() {
        Close();
    }

    // 连接到网络地址
    auto Dial(std::string_view host, const std::uint16_t port) -> bool {
        if (host.empty() || port <= 0) {
            return false;
        }
        remoteAddr_ = host;

        socket_ = std::make_unique<net::TCPConn>();

        if (!socket_->Open()) {
            return false;
        }

        return socket_->Connect(host, port);
    }

    void Run() {}

    auto Read() -> int {
        return 0;
    }

    auto Write() -> int {
        return 0;
    }

    auto SetKeepAlive(bool on) -> bool {
        return socket_->SetKeeplive(on);
    }

    void Close() {
        socket_->Close();
    }

    auto Shutdown() -> bool {
        return socket_->ShutDown();
    }

private:
    // Socket对象
    std::unique_ptr<net::TCPConn> socket_;
    std::chrono::duration<int>    timeout_;
    // 客户端地址
    std::string localAddr_;
    // 远程地址
    std::string                remoteAddr_;
    std::chrono::duration<int> keepAlive_;
};
}  // namespace uranus::net
