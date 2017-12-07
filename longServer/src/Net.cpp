#include "Net.h"

#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

Net::Net() {}

Net::Net(const int fd) : mSocket(fd) {}

Net::~Net() { Close(); }

bool Net::Listen(unsigned short port, std::string_view ip)
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

bool Net::Dial(unsigned short port, std::string_view ip)
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

int Net::Accept()
{
	struct sockaddr_in addr;
	socklen_t socklen = sizeof(addr);
	return ::accept4(mSocket,
					 (struct sockaddr *)&addr,
					 &socklen,
					 SOCK_NONBLOCK | SOCK_CLOEXEC);
}

int Net::SetKeeplive(bool on)
{
	int optVal = on ? 1 : 0;
	return ::setsockopt(mSocket,
						SOL_SOCKET,
						SO_KEEPALIVE,
						&optVal,
						static_cast<socklen_t>(sizeof(optVal)));
}

int Net::ShutDown() { return ::shutdown(mSocket, SHUT_WR); }

int Net::Close() { return ::close(mSocket); }

bool Net::newSocket()
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

bool Net::bindSocket(unsigned short port, std::string_view ip)
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
