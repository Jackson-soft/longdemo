#pragma once

#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string_view>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

class Socket
{
public:
	Socket() {}
	Socket(const int fd) : mSocket(fd) {}
	~Socket() { Close(); }

	bool NewSocket()
	{
		mSocket = ::socket(
			AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
		if (mSocket < 0) {
			return false;
		}

		// reuseport
		int optVal = 1;
		if (::setsockopt(
				mSocket, SOL_SOCKET, SO_REUSEPORT, &optVal, sizeof(optVal))) {
			return false;
		}
		return true;
	}

	//监听服务器
	bool Listen() { return ::listen(mSocket, SOMAXCONN); }

	// 绑定
	int Bind(unsigned short port, std::string_view ip = {""})
	{
		struct sockaddr_in addr;

		std::memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port   = ::htons(port);

		if (!ip.empty()) {
			addr.sin_addr.s_addr = ::inet_addr(ip.data());
		} else {
			addr.sin_addr.s_addr = ::htonl(INADDR_ANY);
		}

		if (INADDR_NONE == addr.sin_addr.s_addr) {
			return 0;
		}

		return ::bind(mSocket, (struct sockaddr *)&addr, sizeof(addr)))
	}

	//连接目标服务器
	int Connect(unsigned short port, std::string_view ip = {""})
	{
		struct sockaddr_in addr;

		std::memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port   = ::htons(port);

		if (!ip.empty()) {
			addr.sin_addr.s_addr = ::inet_addr(ip.data());
		} else {
			addr.sin_addr.s_addr = ::htonl(INADDR_ANY);
		}

		if (INADDR_NONE == addr.sin_addr.s_addr) {
			return 0;
		}

		return ::connect(mSocket, (struct sockaddr *)&addr, sizeof(addr))
	}

	// 返回接收的socket文件描述符
	int Accept()
	{
		struct sockaddr_in addr;
		socklen_t socklen = sizeof(addr);
		return ::accept4(mSocket,
						 (struct sockaddr *)&addr,
						 &socklen,
						 SOCK_NONBLOCK | SOCK_CLOEXEC);
	}

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
	int SetNonBlock(bool on)
	{
		int nFlag = ::fcntl(mSocket, F_GETFL, 0);
		if (nFlag == -1) {
			return nFlag;
		}
		return ::fcntl(mSocket, F_SETFL, nFlag | O_NONBLOCK | FD_CLOEXEC);
	}

	int SetNoDelay(bool on)
	{
		int optval = on ? 1 : 0;
		::setsockopt(mSocket,
					 IPPROTO_TCP,
					 TCP_NODELAY,
					 &optval,
					 static_cast<socklen_t>(sizeof optval));
	}

	void SetReuseAddr(bool on)
	{
		int optval = on ? 1 : 0;
		::setsockopt(mSocket,
					 SOL_SOCKET,
					 SO_REUSEADDR,
					 &optval,
					 static_cast<socklen_t>(sizeof optval));
	}

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

	// 写数据
	ssize_t Write(const void *buf, size_t count)
	{
		return ::send(mSocket, buf, count, 0);
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
	int GetNativeFD() { return mSocket; }

private:
	int mSocket{0}; //
};
