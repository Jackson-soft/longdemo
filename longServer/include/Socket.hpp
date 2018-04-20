#pragma once

// socket封装类,ipv6

#include "Util.hpp"
#include <arpa/inet.h>
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

class Socket : public Noncopyable
{
public:
	Socket() = default;
	Socket(const int fd) : mSocket(fd) {}

	// move constructor
	Socket(const int &&fd) : mSocket(std::move(fd)) {}
	~Socket() { Close(); }

	bool NewSocket(std::string_view network)
	{
		if (network == "tcp") {
			mSocket = ::socket(AF_INET6,
							   SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC,
							   IPPROTO_IP);
		} else if (network == "udp") {
			mSocket = ::socket(AF_INET6,
							   SOCK_DGRAM | SOCK_NONBLOCK | SOCK_CLOEXEC,
							   IPPROTO_IP);
		} else if (network == "unix") {
			mSocket = ::socket(AF_UNIX,
							   SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC,
							   IPPROTO_IP);
		} else {
			return false;
		}

		if (mSocket <= 0) {
			return false;
		}

		mNet = std::move(network);

		return SetReusePort(true) == 1 ? true : false;
	}

	//监听服务器
	int Listen() { return ::listen(mSocket, SOMAXCONN); }

	// 绑定
	int Bind(unsigned short port, std::string_view ip = {""})
	{
		struct sockaddr_in6 addr;

		std::memset(&addr, 0, sizeof(addr));
		addr.sin6_family = AF_INET6;
		addr.sin6_port   = ::htons(port);

		if (!ip.empty()) {
			if (::inet_pton(AF_INET6, ip.data(), &addr.sin6_addr) < 0)
				return 0;
		} else {
			addr.sin6_addr = in6addr_any;
		}

		return ::bind(mSocket,
					  reinterpret_cast<struct sockaddr *>(&addr),
					  static_cast<socklen_t>(sizeof(addr)));
	}

	//连接目标服务器
	int Connect(unsigned short port, std::string_view ip)
	{
		struct sockaddr_in6 addr;

		std::memset(&addr, 0, sizeof(addr));
		addr.sin6_family = AF_INET6;
		addr.sin6_port   = ::htons(port);

		if (!ip.empty()) {
			if (::inet_pton(AF_INET6, ip.data(), &addr.sin6_addr) < 0)
				return 0;
		} else {
			addr.sin6_addr = in6addr_any;
		}

		return ::connect(mSocket,
						 reinterpret_cast<struct sockaddr *>(&addr),
						 static_cast<socklen_t>(sizeof(addr)));
	}

	// 返回接收的socket文件描述符
	int Accept()
	{
		struct sockaddr_in6 addr;
		socklen_t socklen = sizeof(addr);
		std::memset(&addr, 0, socklen);

		return ::accept4(mSocket,
						 reinterpret_cast<struct sockaddr *>(&addr),
						 &socklen,
						 SOCK_NONBLOCK | SOCK_CLOEXEC);
	}

	// 0成功，-1失败
	int SetKeeplive(bool on)
	{
		int optVal = on ? 1 : 0;
		return ::setsockopt(mSocket,
							SOL_SOCKET,
							SO_KEEPALIVE,
							&optVal,
							static_cast<socklen_t>(sizeof(optVal)));
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
	int SetNoDelay(bool on)
	{
		int optval = on ? 1 : 0;
		return ::setsockopt(mSocket,
							IPPROTO_TCP,
							TCP_NODELAY,
							&optval,
							static_cast<socklen_t>(sizeof optval));
	}

	int SetCork(bool on)
	{
		int optVal = on ? 1 : 0;
		return ::setsockopt(mSocket,
							IPPROTO_TCP,
							TCP_CORK,
							&optVal,
							static_cast<socklen_t>(sizeof optVal));
	}

	// 地址复用
	int SetReuseAddr(bool on)
	{
		int optval = on ? 1 : 0;
		return ::setsockopt(mSocket,
							SOL_SOCKET,
							SO_REUSEADDR,
							&optval,
							static_cast<socklen_t>(sizeof optval));
	}

	// 端口复用
	int SetReusePort(bool on)
	{
		int optval = on ? 1 : 0;
		return ::setsockopt(mSocket,
							SOL_SOCKET,
							SO_REUSEPORT,
							&optval,
							static_cast<socklen_t>(sizeof optval));
	}

	// 读数据
	ssize_t Read(void *buf, size_t count)
	{
		return ::recv(mSocket, buf, count, 0);
	}

	ssize_t Readv(const struct iovec *iov, int iovcnt)
	{
		return ::readv(mSocket, iov, iovcnt);
	}

	// 写数据
	ssize_t Write(const void *buf, size_t count)
	{
		return ::send(mSocket, buf, count, 0);
	}

	ssize_t Writev(const struct iovec *iov, int iovcnt)
	{
		return ::writev(mSocket, iov, iovcnt);
	}

	//优雅关闭读写双半闭
	int ShutDown() { return ::shutdown(mSocket, SHUT_RDWR); }

	// 关闭读
	int CloseRead() { return ::shutdown(mSocket, SHUT_RD); }

	// 关闭写
	int CloseWrite() { return ::shutdown(mSocket, SHUT_WR); }

	//关闭
	int Close() { return ::close(mSocket); }

	// 获取原生socket
	int GetNativeFD() const { return mSocket; }

private:
	int mSocket{0}; //

	std::string mNet;
};
