#include <iostream>

#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cerrno>
#include <cstdlib>
#include <cstring>

#include "./message/notify.pb.h"

int main()
{
	struct sockaddr_in servAddr;
	int socketfd = socket(AF_INET, SOCK_STREAM, 0);

	std::memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family		 = AF_INET;
	servAddr.sin_port		 = htons(8088);
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (connect(socketfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
		std::cout << std::strerror(errno) << std::endl;
		std::exit(EXIT_FAILURE);
	}

	notify::Login mLogin;
	mLogin.set_usrname("fusu");
	mLogin.set_password("23455");
	std::cout << "name: " << mLogin.descriptor()->name().data() << std::endl;

	int nSize = mLogin.ByteSize();
	unsigned char bts[nSize];
	mLogin.SerializeToArray(bts, nSize);
	if (::send(socketfd, bts, nSize, 0) < 0) {
		std::cout << std::strerror(errno) << std::endl;
		std::exit(EXIT_FAILURE);
	}

	while (true) {
		// recv();
		char mBuffer[1024];
		int nRecv = ::recv(socketfd, &mBuffer, 1024, MSG_DONTWAIT);
		if (nRecv > 0) {
			std::cout << mBuffer << std::endl;
		}
	}
	close(socketfd);
	return 0;
}
