#pragma once

// socket封装类,ipv6
#include "utils/util.hpp"
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

namespace Uranus
{
namespace Net
{
class Socket : public Uranus::Utils::Noncopyable
{
public:
    Socket() = default;
    Socket(const int fd) : mSocket(fd) {}

    // move constructor
    Socket(const Socket &&obj) {}

    // copy constructor
    Socket(const Socket &obj)
    {
        mSocket = obj.mSocket;
        mNet    = obj.mNet;
    }

    ~Socket() { Close(); }

    bool NewSocket(const std::string_view network = "tcp")
    {
        if (network.empty()) {
            return false;
        } else if (network == "tcp") {
            mSocket = ::socket(AF_INET6, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_IP);
        } else if (network == "udp") {
            mSocket = ::socket(AF_INET6, SOCK_DGRAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_IP);
        } else if (network == "unix") {
            mSocket = ::socket(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_IP);
        } else {
            return false;
        }

        if (mSocket <= 0) {
            return false;
        }

        mNet = std::move(network);

        return SetReusePort(true);
    }

    //监听服务器
    bool Listen() { return ::listen(mSocket, SOMAXCONN) == 0; }

    // 绑定
    bool Bind(const std::uint64_t port, const std::string_view ip = {""})
    {
        if (port <= 0) {
            return false;
        }
        struct sockaddr_in6 addr;

        std::memset(&addr, 0, sizeof(addr));
        addr.sin6_family = AF_INET6;
        addr.sin6_port   = ::htons(port);

        if (!ip.empty()) {
            if (::inet_pton(AF_INET6, ip.data(), &addr.sin6_addr) < 0)
                return false;
        } else {
            addr.sin6_addr = in6addr_any;
        }

        return ::bind(mSocket, reinterpret_cast<struct sockaddr *>(&addr), static_cast<socklen_t>(sizeof(addr))) == 0;
    }

    //连接目标服务器
    bool Connect(const std::string_view ip, const std::uint64_t port)
    {
        if (port <= 0) {
            return false;
        }
        struct sockaddr_in6 addr;

        std::memset(&addr, 0, sizeof(addr));
        addr.sin6_family = AF_INET6;
        addr.sin6_port   = ::htons(port);

        if (!ip.empty()) {
            if (::inet_pton(AF_INET6, ip.data(), &addr.sin6_addr) < 0)
                return false;
        } else {
            addr.sin6_addr = in6addr_any;
        }

        return ::connect(mSocket, reinterpret_cast<struct sockaddr *>(&addr), static_cast<socklen_t>(sizeof(addr)))
               == 0;
    }

    // 返回接收的socket文件描述符
    int Accept()
    {
        struct sockaddr_in6 addr;
        socklen_t socklen = sizeof(addr);
        std::memset(&addr, 0, socklen);

        return ::accept4(mSocket, reinterpret_cast<struct sockaddr *>(&addr), &socklen, SOCK_NONBLOCK | SOCK_CLOEXEC);
    }

    // 0成功，-1失败
    bool SetKeeplive(bool on)
    {
        int optVal = on ? 1 : 0;
        return ::setsockopt(mSocket, SOL_SOCKET, SO_KEEPALIVE, &optVal, static_cast<socklen_t>(sizeof(optVal))) == 0;
    }

    // 设置非阻塞
    int SetNonBlock()
    {
        int nFlag = ::fcntl(mSocket, F_GETFL, 0);
        if (nFlag == -1) {
            return nFlag;
        }
        return ::fcntl(mSocket, F_SETFL, nFlag | O_NONBLOCK | FD_CLOEXEC);
    }

    // Nagle 算法(小包合并成大包)
    bool SetNoDelay(bool on)
    {
        int optval = on ? 1 : 0;
        return ::setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, &optval, static_cast<socklen_t>(sizeof optval)) == 0;
    }

    bool SetCork(bool on)
    {
        int optVal = on ? 1 : 0;
        return ::setsockopt(mSocket, IPPROTO_TCP, TCP_CORK, &optVal, static_cast<socklen_t>(sizeof optVal)) == 0;
    }

    // 地址复用
    bool SetReuseAddr(bool on)
    {
        int optval = on ? 1 : 0;
        return ::setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, &optval, static_cast<socklen_t>(sizeof optval)) == 0;
    }

    // 端口复用
    bool SetReusePort(bool on)
    {
        int optval = on ? 1 : 0;
        return ::setsockopt(mSocket, SOL_SOCKET, SO_REUSEPORT, &optval, static_cast<socklen_t>(sizeof optval)) == 0;
    }

    // 读数据
    ssize_t Read(void *buf, size_t count) { return ::recv(mSocket, buf, count, 0); }

    ssize_t Readv(const struct iovec *iov, int iovcnt) { return ::readv(mSocket, iov, iovcnt); }

    // 写数据
    ssize_t Write(const void *buf, size_t count) { return ::send(mSocket, buf, count, 0); }

    ssize_t Writev(const struct iovec *iov, int iovcnt) { return ::writev(mSocket, iov, iovcnt); }

    //优雅关闭读写双半闭
    bool ShutDown() { return ::shutdown(mSocket, SHUT_RDWR) == 0; }

    // 关闭读
    bool CloseRead() { return ::shutdown(mSocket, SHUT_RD) == 0; }

    // 关闭写
    bool CloseWrite() { return ::shutdown(mSocket, SHUT_WR) == 0; }

    //关闭
    bool Close() { return ::close(mSocket) == 0; }

    // 获取原生socket
    int GetNativeFD() const { return mSocket; }

private:
    void GetTcpInfo() {}

private:
    int mSocket{0};  //

    std::string mNet{""};
};
}  // namespace Net
}  // namespace Uranus