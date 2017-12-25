#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string_view>

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

int main()
{
	int socketfd = ::socket(AF_UNIX, SOCK_STREAM, 0);
	if (socketfd < 0) {
		std::exit(-1);
	}

	struct sockaddr_un svrAddr;
	std::string_view sPath{"/run/echo.sock"};

	std::memset(&svrAddr, 0, sizeof(svrAddr));
	svrAddr.sun_family = AF_UNIX;
	std::strcpy(svrAddr.sun_path, sPath.data());

	::unlink(sPath.data());

	if (::bind(socketfd, (struct sockaddr *)&svrAddr, sizeof(svrAddr)) < 0) {
		std::exit(-1);
	}

	::chmod(sPath.data(), 00640);

	if (::listen(socketfd, SOMAXCONN) < 0) {
		std::exit(-1);
	}

	std::string_view sBuf{"this is a message!"};

	int connfd{0};
	char buf[1024];
	while (true) {
		if ((connfd = ::accept4(socketfd, nullptr, nullptr, SOCK_NONBLOCK)) <
			0) {
			continue;
		}

		::recv(connfd, buf, 1024, 0);
		std::cout << buf << std::endl;

		::send(connfd, sBuf.data(), sizeof(sBuf), 0);
	}

	::close(socketfd);
	::close(connfd);
	return 0;
}