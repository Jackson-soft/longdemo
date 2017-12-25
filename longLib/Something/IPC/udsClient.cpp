#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string_view>

#include <netinet/in.h>
#include <sys/socket.h>
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
	std::memset(&svrAddr, 0, sizeof(svrAddr));
	svrAddr.sun_family = AF_UNIX;
	std::strcpy(svrAddr.sun_path, "/run/echo.sock");

	if (::connect(socketfd, (struct sockaddr *)&svrAddr, sizeof(svrAddr)) < 0) {
		std::exit(-1);
	}

	std::string_view buf{"this is a message!"};

	::send(socketfd, buf.data(), sizeof(buf), 0);

	// recv();

	::close(socketfd);
	return 0;
}