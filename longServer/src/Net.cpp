#include "Net.h"

#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

Net::Net() {}

Net::~Net() { Close(); }

bool Net::Listen(const char *ip, unsigned short port)
{
	fSocket = ::socket(
		AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
	if (fSocket < 0) {
		return false;
	}

	// reuseport
	int opt_val = 1;
	if (::setsockopt(
			fSocket, SOL_SOCKET, SO_REUSEPORT, &opt_val, sizeof(opt_val))) {
		return false;
	}
	struct sockaddr_in addr;
	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family		 = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port		 = htons(port);

	bool bRet = true;
	if ('\0' != *ip) {
		addr.sin_addr.s_addr = ::inet_addr(ip);
		if (INADDR_NONE == addr.sin_addr.s_addr) {
			bRet = false;
		}
	}
	if (bRet) {
		if (::bind(fSocket, (struct sockaddr *)&addr, sizeof(addr))) {
			bRet = false;
		}
	}
	if (bRet) {
		if (::listen(fSocket, SOMAXCONN)) {
			bRet = false;
		}
	}

	//防止内存泄漏
	if (!bRet && fSocket >= 0) {
		::close(fSocket);
	}
	return bRet;
}

int Net::Accept()
{
	struct sockaddr_in addr;
	socklen_t socklen = sizeof(addr);
	return ::accept4(fSocket,
					 (struct sockaddr *)&addr,
					 &socklen,
					 SOCK_NONBLOCK | SOCK_CLOEXEC);
}

int Net::ShutDown() { return ::shutdown(fSocket, SHUT_WR); }

int Net::Close() { return ::close(fSocket); }
