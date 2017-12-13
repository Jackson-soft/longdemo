#pragma once

#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <netinet/in.h>
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

	//监听服务器
	bool Listen(unsigned short port, std::string_view ip = {""})
	{
		bool bRet = newSocket();
		if (bRet) {
			bRet = bindSocket(port, ip);
		}

		if (bRet) {
			if (::listen(mSocket, SOMAXCONN)) {
				bRet = false;
			}
		}

		return bRet;
	}

	//连接目标服务器
	bool Dial(unsigned short port, std::string_view ip)
	{
		bool bRet = newSocket();

		if (bRet) {
			struct sockaddr_in servAddr;

			std::memset(&servAddr, 0, sizeof(servAddr));
			servAddr.sin_family		 = AF_INET;
			servAddr.sin_port		 = ::htons(port);
			servAddr.sin_addr.s_addr = ::inet_addr(ip.data());
			if (::connect(
					mSocket, (struct sockaddr *)&servAddr, sizeof(servAddr))) {
				bRet = false;
			}
		}

		return bRet;
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

	//优雅关闭
	int ShutDown() { return ::shutdown(mSocket, SHUT_WR); }

	//关闭
	int Close() { return ::close(mSocket); }

private:
	int mSocket{0}; //

	//
	bool newSocket()
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
	}

	//
	bool bindSocket(unsigned short port, std::string_view ip = {""})
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
			return false;
		}

		if (::bind(mSocket, (struct sockaddr *)&addr, sizeof(addr))) {
			return false;
		}
		return true;
	}
};
