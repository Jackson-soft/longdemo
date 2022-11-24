#pragma once

// socket封装类,ipv6
#include "net/connect.hpp"

#include <arpa/inet.h>
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

namespace uranus::net {
class UDPConn : public Conn {
public:
    explicit UDPConn() = default;

    // move constructor
    UDPConn(UDPConn &&obj) noexcept : socket_(obj.socket_) {}

    // copy constructor
    UDPConn(const UDPConn &obj) = default;

    auto operator=(const UDPConn &obj) -> UDPConn & {
        if (this != &obj) {
            socket_  = obj.socket_;
            network_ = obj.network_;
        }
        return *this;
    }

    auto operator=(UDPConn &&obj) noexcept -> UDPConn & {
        socket_  = obj.socket_;
        network_ = obj.network_;
        return *this;
    }

    ~UDPConn() override {
        Close();
    }

    auto Open() -> bool {
        socket_ = ::socket(AF_INET6, SOCK_DGRAM, IPPROTO_IP);
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

    bool SetCork(bool on) {
        int optVal = on ? 1 : 0;
        return ::setsockopt(socket_, IPPROTO_TCP, TCP_CORK, &optVal, static_cast<socklen_t>(sizeof optVal)) == 0;
    }

    // 地址复用
    bool SetReuseAddr(bool on) {
        int optval = on ? 1 : 0;
        return ::setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &optval, static_cast<socklen_t>(sizeof optval)) == 0;
    }

    // 端口复用
    bool SetReusePort(bool on) {
        int optval = on ? 1 : 0;
        return ::setsockopt(socket_, SOL_SOCKET, SO_REUSEPORT, &optval, static_cast<socklen_t>(sizeof optval)) == 0;
    }

    // 读数据
    ssize_t Read(void *buf, size_t count) {
        return ::recv(socket_, buf, count, 0);
    }

    ssize_t Readv(const struct iovec *iov, int iovcnt) {
        return ::readv(socket_, iov, iovcnt);
    }

    // 写数据
    ssize_t Write(const void *buf, size_t count) {
        return ::send(socket_, buf, count, 0);
    }

    ssize_t Writev(const struct iovec *iov, int iovcnt) {
        return ::writev(socket_, iov, iovcnt);
    }

    // 优雅关闭读写双半闭
    bool ShutDown() {
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
    [[nodiscard]] auto Close() const -> bool {
        return ::close(socket_) == 0;
    }

    // 获取原生socket
    [[nodiscard]] auto Native() const -> int {
        return socket_;
    }

private:
    void getTcpInfo() {}

    int socket_{0};
};
}  // namespace uranus::net