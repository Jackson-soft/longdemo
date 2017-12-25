#include <fcntl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

#include "./message/notify.pb.h"

int main()
{
	int socketfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in servAddr;
	std::memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family		 = AF_INET;
	servAddr.sin_port		 = htons(8088);
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int on = 1;
	::setsockopt(socketfd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));
	::bind(socketfd, (struct sockaddr *)&servAddr, sizeof(servAddr));

	// 设置非阻塞
	int nFlag = ::fcntl(socketfd, F_GETFL, 0);
	if (nFlag == -1) {
		std::cout << "fcntl: " << std::strerror(errno) << std::endl;
		std::exit(-1);
	}
	if (::fcntl(socketfd, F_SETFL, nFlag | O_NONBLOCK) == -1) {
		std::cout << "fcntl2: " << std::strerror(errno) << std::endl;
		std::exit(-1);
	}

	::listen(socketfd, SOMAXCONN);

	int epollfd = epoll_create1(EPOLL_CLOEXEC);
	if (epollfd <= 0) {
		std::cout << "epoll_create1: " << std::strerror(errno) << std::endl;
		std::exit(-1);
	}

	struct epoll_event epEvent;
	epEvent.data.fd = socketfd;
	epEvent.events  = EPOLLIN | EPOLLOUT;
	::epoll_ctl(epollfd, EPOLL_CTL_ADD, socketfd, &epEvent);

	using EventList = std::vector<struct epoll_event>;
	EventList events(16);
	struct sockaddr_in peerAddr;
	socklen_t peerLen;
	int conn;

	int nReady;
	while (1) {
		nReady = ::epoll_wait(
			epollfd, &*events.begin(), static_cast<int>(events.size()), -1);
		if (nReady <= 0) {
			continue;
		}

		if ((size_t)nReady == events.size()) {
			events.resize(events.size() * 2);
		}

		for (int i = 0; i < nReady; ++i) {
			if ((events[i].events & EPOLLERR) ||
				(events[i].events & EPOLLHUP) ||
				(!(events[i].events & EPOLLIN))) {
				::close(events[i].data.fd);
				continue;
			} else if (events[i].data.fd == socketfd) {
				//处理新连接
				peerLen = sizeof(peerAddr);
				conn	= ::accept4(socketfd,
								 (struct sockaddr *)&peerAddr,
								 &peerLen,
								 SOCK_NONBLOCK);
				if (conn <= 0) {
					continue;
				}

				epEvent.data.fd = conn;
				epEvent.events  = EPOLLIN;
				::epoll_ctl(epollfd, EPOLL_CTL_ADD, conn, &epEvent);
			} else if (events[i].events & EPOLLIN) {
				// read
				conn = events[i].data.fd;
				if (conn <= 0) {
					continue;
				}
				char recvBuf[1024]{0};
				int ret = ::recv(conn, recvBuf, 1024, MSG_DONTWAIT);
				if (ret == 0) {
					std::cout << "client close" << std::endl;

					epEvent = events[i];
					::epoll_ctl(epollfd, EPOLL_CTL_DEL, conn, &epEvent);
					::close(conn);
				}

				notify::Login mLogin;
				mLogin.ParseFromArray(recvBuf, ret);
				std::cout << mLogin.usrname().data() << std::endl;
				std::cout << mLogin.password().data() << std::endl;
				std::cout << "name: " << mLogin.descriptor()->name().data()
						  << std::endl;
				// epEvent			= events[i];
				epEvent.events  = EPOLLOUT;
				epEvent.data.fd = conn;
				if (::epoll_ctl(epollfd, EPOLL_CTL_MOD, conn, &epEvent) < 0) {
					std::cout << "read: " << std::strerror(errno) << std::endl;
				}
			} else if (events[i].events & EPOLLOUT) {
				// write
				std::cout << "write" << std::endl;
				conn = events[i].data.fd;
				if (conn <= 0) {
					continue;
				}

				std::string mSend{"hi,client!!!"};
				::send(conn, mSend.data(), mSend.size(), MSG_DONTWAIT);

				// epEvent			= events[i];
				epEvent.events  = EPOLLIN;
				epEvent.data.fd = conn;
				if (::epoll_ctl(epollfd, EPOLL_CTL_MOD, conn, &epEvent) < 0) {
					std::cout << "write: " << std::strerror(errno) << std::endl;
				}
			}
		}
	}
	::close(epollfd);
	return 0;
}
