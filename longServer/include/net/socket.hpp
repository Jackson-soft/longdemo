#pragma once

// socket封装类,ipv6
#include "utils/noncopyable.hpp"
#include <arpa/inet.h>
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

namespace Uranus::Net
{
class Socket : public Utils::Noncopyable
{
public:
    Socket() = default;
    Socket(const int sfd, std::string_view net) : fd(sfd), network(net) {}

    // move constructor
    Socket(const Socket &&obj) = default;

    // copy constructor
    Socket(const Socket &obj)
    {
        fd      = obj.fd;
        network = obj.network;
    }

    ~Socket() { Close(); }

    bool NewSocket(std::string_view net = "tcp")
    {
        if (net.empty()) {
            return false;
        }
        if (net == "tcp") {
            fd = ::socket(PF_INET6, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_IP);
        } else if (net == "udp") {
            fd = ::socket(PF_INET6, SOCK_DGRAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_IP);
        } else if (net == "unix") {
            fd = ::socket(PF_UNIX, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_IP);
        } else {
            return false;
        }

        if (fd <= 0) {
            ::close(fd);
            return false;
        }

        network = net;

        return true;
    }

    //监听服务器
    bool Listen() { return ::listen(fd, SOMAXCONN) == 0; }

    // 绑定
    bool Bind(const std::uint16_t port, std::string_view ip = {""})
    {
        if (port <= 0) {
            return false;
        }

        struct sockaddr_in6 addr {
        };

        std::memset(&addr, 0, sizeof(addr));
        addr.sin6_family = AF_INET6;
        addr.sin6_port   = ::htons(port);

        if (!ip.empty()) {
            if (::inet_pton(AF_INET6, ip.data(), &addr.sin6_addr) < 0)
                return false;
        } else {
            addr.sin6_addr = in6addr_any;
        }

        return ::bind(fd, reinterpret_cast<struct sockaddr *>(&addr), static_cast<socklen_t>(sizeof(addr))) == 0;
    }

    //连接目标服务器
    bool Connect(std::string_view ip, const std::uint16_t port)
    {
        if (port <= 0) {
            return false;
        }
        struct sockaddr_in6 addr {
        };

        std::memset(&addr, 0, sizeof(addr));
        addr.sin6_family = AF_INET6;
        addr.sin6_port   = ::htons(port);

        if (!ip.empty()) {
            if (::inet_pton(AF_INET6, ip.data(), &addr.sin6_addr) <= 0)
                return false;
        } else {
            addr.sin6_addr = in6addr_any;
        }

        return ::connect(fd, reinterpret_cast<struct sockaddr *>(&addr), static_cast<socklen_t>(sizeof(addr))) == 0;
    }

    // 返回接收的socket文件描述符
    int Accept()
    {
        struct sockaddr_in6 addr {
        };
        socklen_t socklen = sizeof(addr);
        std::memset(&addr, 0, socklen);

        return ::accept4(fd, reinterpret_cast<struct sockaddr *>(&addr), &socklen, SOCK_NONBLOCK | SOCK_CLOEXEC);
    }

    // 0成功，-1失败
    bool Keepalive(bool on)
    {
        int optVal = on ? 1 : 0;
        return ::setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &optVal, static_cast<socklen_t>(sizeof(optVal))) == 0;
    }

    // 设置非阻塞
    bool NonBlock()
    {
        int flag = ::fcntl(fd, F_GETFL, 0);
        if (flag < 0) {
            return false;
        }
        return ::fcntl(fd, F_SETFL, flag | O_NONBLOCK | FD_CLOEXEC) < 0;
    }

    // Nagle 算法(小包合并成大包)
    bool NoDelay(bool on)
    {
        int optval = on ? 1 : 0;
        return ::setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &optval, static_cast<socklen_t>(sizeof optval)) == 0;
    }

    bool Cork(bool on)
    {
        int optVal = on ? 1 : 0;
        return ::setsockopt(fd, IPPROTO_TCP, TCP_CORK, &optVal, static_cast<socklen_t>(sizeof optVal)) == 0;
    }

    // 地址复用
    bool ReuseAddr(bool on)
    {
        int optval = on ? 1 : 0;
        return ::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, static_cast<socklen_t>(sizeof optval)) == 0;
    }

    // 端口复用
    bool ReusePort(bool on)
    {
        int optval = on ? 1 : 0;
        return ::setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &optval, static_cast<socklen_t>(sizeof optval)) == 0;
    }

    // 读数据
    ssize_t Read(void *buf, size_t count) { return ::recv(fd, buf, count, 0); }

    ssize_t Readv(const struct iovec *iov, int iovcnt) { return ::readv(fd, iov, iovcnt); }

    // 写数据
    ssize_t Write(const void *buf, size_t count) { return ::send(fd, buf, count, 0); }

    ssize_t Writev(const struct iovec *iov, int iovcnt) { return ::writev(fd, iov, iovcnt); }

    //优雅关闭读写双半闭
    bool Shutdown() { return ::shutdown(fd, SHUT_RDWR) == 0; }

    // 关闭读
    bool CloseRead() { return ::shutdown(fd, SHUT_RD) == 0; }

    // 关闭写
    bool CloseWrite() { return ::shutdown(fd, SHUT_WR) == 0; }

    //关闭
    bool Close() { return ::close(fd) == 0; }

    // 远程地址和端口
    auto Remote() -> std::pair<std::string, std::uint16_t>
    {
        struct sockaddr_in6 addr {
        };
        socklen_t addrLen = sizeof(addr);
        std::memset(&addr, 0, addrLen);

        if (::getpeername(fd, (struct sockaddr *)&addr, &addrLen) < 0)
            return {"", 0};
        char *str{};
        if (::inet_ntop(AF_INET6, &addr.sin6_addr, str, INET6_ADDRSTRLEN) == nullptr)
            return {"", 0};
        auto port = ::ntohs(addr.sin6_port);

        std::string ip(str, INET6_ADDRSTRLEN);
        return {ip, port};
    }

    // 本地地址和端口
    auto Local() -> std::pair<std::string, std::uint16_t>
    {
        struct sockaddr_in6 addr {
        };
        socklen_t addrLen = sizeof(addr);
        std::memset(&addr, 0, addrLen);

        if (::getsockname(fd, (struct sockaddr *)&addr, &addrLen) < 0)
            return {"", 0};

        char *str{};
        if (::inet_ntop(AF_INET6, &addr.sin6_addr, str, INET6_ADDRSTRLEN) == nullptr)
            return {"", 0};
        auto port = ::ntohs(addr.sin6_port);
        std::string ip{str, INET6_ADDRSTRLEN};
        return {ip, port};
    }

    // 获取原生socket
    [[nodiscard]] auto NativeFD() const -> int { return fd; }

private:
    void GetTcpInfo() {}

    int fd{0};                // socket 描述符
    std::string network{""};  // 网络类型
};
}  // namespace Uranus::Net
