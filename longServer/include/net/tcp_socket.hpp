#pragma once

// socket封装类,ipv6
#include "net/connect.hpp"

#include <arpa/inet.h>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string>
#include <string_view>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <utility>
#include <vector>

namespace uranus::net {
class TCPConn : public Conn {
public:
    explicit TCPConn() = default;

    // move constructor
    TCPConn(TCPConn &&obj) noexcept : socket_(obj.socket_) {}

    // copy constructor
    TCPConn(const TCPConn &obj) = default;

    auto operator=(const TCPConn &obj) -> TCPConn & {
        if (this != &obj) {
            socket_ = obj.socket_;
        }
        return *this;
    }

    auto operator=(TCPConn &&obj) noexcept -> TCPConn & {
        socket_ = obj.socket_;
        return *this;
    }

    ~TCPConn() override {
        Close();
    }

    auto Open() -> bool {
        socket_ = ::socket(AF_INET6, SOCK_STREAM, IPPROTO_IP);
        return socket_ > 0;
    }

    // 监听服务器
    [[nodiscard]] auto Listen() const -> bool {
        return ::listen(socket_, SOMAXCONN) == 0;
    }

    // 绑定
    [[nodiscard]] auto Bind(const std::uint16_t port, std::string_view host = {""}) const -> bool {
        if (port <= 0) {
            return false;
        }

        struct sockaddr_in6 addr {};

        std::memset(&addr, 0, sizeof(addr));
        addr.sin6_family = AF_INET6;
        addr.sin6_port   = ::htons(port);

        if (!host.empty()) {
            if (::inet_pton(AF_INET6, host.data(), &addr.sin6_addr) < 0) {
                return false;
            }
        } else {
            addr.sin6_addr = in6addr_any;
        }

        return ::bind(socket_, reinterpret_cast<struct sockaddr *>(&addr), static_cast<socklen_t>(sizeof(addr))) == 0;
    }

    // 连接目标服务器
    [[nodiscard]] auto Connect(std::string_view ip, const std::uint16_t port) const -> bool {
        if (port <= 0) {
            return false;
        }

        struct sockaddr_in6 addr {};

        std::memset(&addr, 0, sizeof(addr));
        addr.sin6_family = AF_INET6;
        addr.sin6_port   = ::htons(port);

        if (!ip.empty()) {
            if (::inet_pton(AF_INET6, ip.data(), &addr.sin6_addr) < 0) {
                return false;
            }
        } else {
            addr.sin6_addr = in6addr_any;
        }

        return ::connect(socket_, reinterpret_cast<struct sockaddr *>(&addr), static_cast<socklen_t>(sizeof(addr)))
            == 0;
    }

    // 返回接收的socket文件描述符
    auto Accept() -> int {
        struct sockaddr_in6 addr {};

        socklen_t socklen = sizeof(addr);
        std::memset(&addr, 0, socklen);

        return ::accept4(socket_, reinterpret_cast<struct sockaddr *>(&addr), &socklen, SOCK_NONBLOCK | SOCK_CLOEXEC);
    }

    // 0成功，-1失败
    [[nodiscard]] auto SetKeeplive(bool on) const -> bool {
        int optVal = on ? 1 : 0;
        return ::setsockopt(socket_, SOL_SOCKET, SO_KEEPALIVE, &optVal, static_cast<socklen_t>(sizeof(optVal))) == 0;
    }

    // 设置非阻塞
    [[nodiscard]] auto SetNonBlock() const -> int {
        int nFlag = ::fcntl(socket_, F_GETFL, 0);
        if (nFlag == -1) {
            return nFlag;
        }
        return ::fcntl(socket_, F_SETFL, nFlag | O_NONBLOCK);
    }

    // Nagle 算法(小包合并成大包)
    [[nodiscard]] auto SetNoDelay(bool on) const -> bool {
        int optval = on ? 1 : 0;
        return ::setsockopt(socket_, IPPROTO_TCP, TCP_NODELAY, &optval, static_cast<socklen_t>(sizeof optval)) == 0;
    }

    auto SetCork(bool on) -> bool {
        int optVal = on ? 1 : 0;
        return ::setsockopt(socket_, IPPROTO_TCP, TCP_CORK, &optVal, static_cast<socklen_t>(sizeof optVal)) == 0;
    }

    // 地址复用
    [[nodiscard]] auto SetReuseAddr(bool on) const -> bool {
        int optval = on ? 1 : 0;
        return ::setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &optval, static_cast<socklen_t>(sizeof optval)) == 0;
    }

    // 端口复用
    [[nodiscard]] auto SetReusePort(bool on) const -> bool {
        int optval = on ? 1 : 0;
        return ::setsockopt(socket_, SOL_SOCKET, SO_REUSEPORT, &optval, static_cast<socklen_t>(sizeof optval)) == 0;
    }

    // 读数据
    auto Read(std::vector<std::byte> data) -> std::size_t override {
        return ::recv(socket_, data.data(), data.size(), MSG_WAITALL);
    }

    // 写数据
    auto Write(std::vector<std::byte> data) -> std::size_t override {
        return ::send(socket_, data.data(), data.size(), MSG_WAITALL);
    }

    // 优雅关闭读写双半闭
    [[nodiscard]] auto ShutDown() const -> bool {
        return ::shutdown(socket_, SHUT_RDWR) == 0;
    }

    // 关闭读
    [[nodiscard]] auto CloseRead() const -> bool {
        return ::shutdown(socket_, SHUT_RD) == 0;
    }

    // 关闭写
    [[nodiscard]] auto CloseWrite() const -> bool {
        return ::shutdown(socket_, SHUT_WR) == 0;
    }

    // 关闭
    auto Close() -> bool override {
        return ::close(socket_) == 0;
    }

    // 获取原生socket
    [[nodiscard]] auto Native() const -> int {
        return socket_;
    }

private:
    int socket_{0};
};
}  // namespace uranus::net
