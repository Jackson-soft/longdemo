#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <unistd.h>

#include <cstring>

#include <vector>
#include <algorithm>
#include <iostream>

int main()
{
    int socketfd;
    socketfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in servAddr;
    std::memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(8088);
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int on = 1;
    ::setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    ::bind(socketfd, (struct sockaddr *)&servAddr, sizeof(servAddr));
    ::listen(socketfd, SOMAXCONN);
    std::vector<int> clients;

    int epollfd = epoll_create1(EPOLL_CLOEXEC);
    struct epoll_event epEvent;
    epEvent.data.fd = socketfd;
    epEvent.events = EPOLLIN | EPOLLET;
    ::epoll_ctl(epollfd, EPOLL_CTL_ADD, socketfd, &epEvent);

    using EventList = std::vector<struct epoll_event>;
    EventList events(16);
    struct sockaddr_in peerAddr;
    socklen_t peerLen;
    int conn;

    int nReady;
    while (1) {
        nReady = ::epoll_wait(epollfd, &*events.begin(), static_cast<int>(events.size()), -1);
        if (nReady == -1) {
            continue;
        }

        if ((size_t)nReady == events.size()) {
            events.resize(events.size() * 2);
        }

        for(int i = 0; i < nReady; ++i) {
            if(events[i].data.fd == socketfd) {
                peerLen = sizeof(peerAddr);
                conn = ::accept4(socketfd, (struct sockaddr *)&peerAddr, &peerLen, SOCK_NONBLOCK);
                if(conn == -1) {
                    std::cout << "ddd" << std::endl;
                }
                clients.emplace_back(conn);

                epEvent.data.fd = conn;
                epEvent.events = EPOLLIN | EPOLLET;
                ::epoll_ctl(epollfd, EPOLL_CTL_ADD, conn, &epEvent);
            } else if(events[i].events & EPOLLIN) {
                conn = events[i].data.fd;
                if(conn < 0) {
                    continue;
                }
                char recvBuf[1024]{0};
                int ret = ::recv(conn, recvBuf, 1024, MSG_DONTWAIT);
                if(ret == 0) {
                    std::cout << "client close" << std::endl;
                    ::close(conn);

                    epEvent = events[i];
                    ::epoll_ctl(epollfd, EPOLL_CTL_DEL, conn, &epEvent);
                    clients.erase(std::remove(clients.begin(), clients.end(), conn), clients.end());
                }

                ::write(conn, recvBuf, std::strlen(recvBuf));
            }
        }
    }
    return 0;
}
