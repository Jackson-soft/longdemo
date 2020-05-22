#pragma once

// socket封装类
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
class Socket
{
public:
    Socket() = default;
    Socket(const int sfd, std::string_view net) : fd(sfd), network(net) {}

    // move constructor
    Socket(Socket &&obj) = default;

    // copy constructor
    Socket(const Socket &obj)
    {
        fd      = obj.fd;
        network = obj.network;
    }

    ~Socket() { Close(); }

    auto Create(std::string_view net = "tcp") -> bool
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
    auto Listen() -> bool { return ::listen(fd, SOMAXCONN) == 0; }

    // 绑定
    auto Bind(const std::uint16_t port, std::string_view ip = {""}) -> bool
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
    auto Connect(std::string_view ip, const std::uint16_t port) -> bool
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
    auto Accept() -> int
    {
        struct sockaddr_in6 addr {
        };
        socklen_t socklen = sizeof(addr);
        std::memset(&addr, 0, socklen);

        return ::accept4(fd, reinterpret_cast<struct sockaddr *>(&addr), &socklen, SOCK_NONBLOCK | SOCK_CLOEXEC);
    }

    // 0成功，-1失败
    auto Keepalive(bool on) -> bool
    {
        int optVal = on ? 1 : 0;
        return ::setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &optVal, static_cast<socklen_t>(sizeof(optVal))) == 0;
    }

    // 设置非阻塞
    auto NonBlock() -> bool
    {
        int flag = ::fcntl(fd, F_GETFL, 0);
        if (flag < 0) {
            return false;
        }
        return ::fcntl(fd, F_SETFL, flag | O_NONBLOCK | FD_CLOEXEC) < 0;
    }

    // Nagle 算法(小包合并成大包)
    auto NoDelay(bool on) -> bool
    {
        int optval = on ? 1 : 0;
        return ::setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &optval, static_cast<socklen_t>(sizeof(optval))) == 0;
    }

    auto Cork(bool on) -> bool
    {
        int optVal = on ? 1 : 0;
        return ::setsockopt(fd, IPPROTO_TCP, TCP_CORK, &optVal, static_cast<socklen_t>(sizeof(optVal))) == 0;
    }

    // 地址复用
    auto ReuseAddr(bool on) -> bool
    {
        int optval = on ? 1 : 0;
        return ::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, static_cast<socklen_t>(sizeof optval)) == 0;
    }

    // 端口复用
    auto ReusePort(bool on) -> bool
    {
        int optval = on ? 1 : 0;
        return ::setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &optval, static_cast<socklen_t>(sizeof optval)) == 0;
    }

    // 读数据
    auto Read(void *buf, size_t count) -> int { return ::recv(fd, buf, count, 0); }

    auto Readv(const struct iovec *iov, int iovcnt) -> int { return ::readv(fd, iov, iovcnt); }

    // 写数据
    auto Write(const void *buf, size_t count) -> int { return ::send(fd, buf, count, 0); }

    auto Writev(const struct iovec *iov, int iovcnt) -> int { return ::writev(fd, iov, iovcnt); }

    //优雅关闭读写双半闭
    auto Shutdown() -> bool { return ::shutdown(fd, SHUT_RDWR) == 0; }

    // 关闭读
    auto CloseRead() -> bool { return ::shutdown(fd, SHUT_RD) == 0; }

    // 关闭写
    auto CloseWrite() -> bool { return ::shutdown(fd, SHUT_WR) == 0; }

    //关闭
    auto Close() -> bool { return ::close(fd) == 0; }

    // 远程地址和端口
    auto Remote() -> std::pair<std::string, std::uint16_t>
    {
        struct sockaddr_in6 addr {
        };
        socklen_t addrLen = sizeof(addr);
        std::memset(&addr, 0, addrLen);

        if (::getpeername(fd, reinterpret_cast<struct sockaddr *>(&addr), &addrLen) < 0)
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

        if (::getsockname(fd, reinterpret_cast<struct sockaddr *>(&addr), &addrLen) < 0)
            return {"", 0};

        char *str{};
        if (::inet_ntop(AF_INET6, &addr.sin6_addr, str, INET6_ADDRSTRLEN) == nullptr)
            return {"", 0};

        auto port = ::ntohs(addr.sin6_port);
        std::string ip{str, INET6_ADDRSTRLEN};
        return {ip, port};
    }

    // 获取原生socket
    [[nodiscard]] auto Native() const -> int { return fd; }

private:
    void GetTcpInfo() {}

    int fd{0};                // socket 描述符
    std::string network{""};  // 网络类型
};
}  // namespace Uranus::Net
