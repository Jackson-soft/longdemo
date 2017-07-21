#include "Net.h"

#include <cstring>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

Net::Net() {}

Net::~Net() { Close(); }

bool Net::InitNet()
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
	struct sockaddr_in service;
	std::memset(&service, 0, sizeof(service));
	service.sin_family		= AF_INET;
	service.sin_addr.s_addr = htonl(INADDR_ANY);
	service.sin_port		= htons(8080);

	if (::bind(fSocket, (struct sockaddr *)&service, sizeof(service))) {
		return false;
	}

	if (::listen(fSocket, SOMAXCONN)) {
		return false;
	}

	return true;
}

int Net::Accept()
{
	return ::accept4(fSocket,
					 (struct sockaddr *)nullptr,
					 NULL,
					 SOCK_NONBLOCK | SOCK_CLOEXEC);
}

int Net::ShutDown() { return ::shutdown(fSocket, SHUT_WR); }

int Net::Close() { return ::close(fSocket); }
