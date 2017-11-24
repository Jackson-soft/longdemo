#include "Net.h"

#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

Socket::Socket() {}

Socket::~Socket() { Close(); }

bool Socket::Listen(std::string_view ip, unsigned short port)
{
    mSocket = ::socket(
		AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if (mSocket < 0) {
		return false;
	}

	// reuseport
	int opt_val = 1;
	if (::setsockopt(
            mSocket, SOL_SOCKET, SO_REUSEPORT, &opt_val, sizeof(opt_val))) {
		return false;
	}
	struct sockaddr_in addr;
	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family		 = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port		 = htons(port);

	bool bRet = true;
	if (!ip.empty()) {
		addr.sin_addr.s_addr = ::inet_addr(ip.data());
		if (INADDR_NONE == addr.sin_addr.s_addr) {
			bRet = false;
		}
	}
	if (bRet) {
        if (::bind(mSocket, (struct sockaddr *)&addr, sizeof(addr))) {
			bRet = false;
		}
	}
	if (bRet) {
        if (::listen(mSocket, SOMAXCONN)) {
			bRet = false;
		}
	}

	//防止内存泄漏
    if (!bRet && mSocket >= 0) {
        ::close(mSocket);
	}
	return bRet;
}

int Socket::Accept()
{
	struct sockaddr_in addr;
	socklen_t socklen = sizeof(addr);
    return ::accept4(mSocket,
					 (struct sockaddr *)&addr,
					 &socklen,
					 SOCK_NONBLOCK | SOCK_CLOEXEC);
}

int Socket::ShutDown() { return ::shutdown(mSocket, SHUT_WR); }

int Socket::Close() { return ::close(mSocket); }
