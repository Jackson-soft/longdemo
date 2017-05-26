#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>

#include <string>

#include "src/ThreadPool.h"

auto fun(int x, int y)
{
    return x + y;
}

int main()
{
    ThreadPool pool{5};
    auto f = pool.Commit(fun, 3, 5);
    std::cout << f.get() << std::endl;
    pool.Stop();
    int socketfd = ::socket(AF_INET, SOCK_STREAM, 0);
    if(socketfd < 0) {
        std::exit(EXIT_FAILURE);
    }
    struct sockaddr_in servAddr;
    std::memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(8088);

    if(::bind(socketfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0){
        std::cout << std::strerror(errno) << std::endl;
        std::exit(EXIT_FAILURE);
    }
    if(listen(socketfd, 10) < 0) {
        std::cout << std::strerror(errno) << std::endl;
        std::exit(EXIT_FAILURE);
    }
    int connectfd;
    char buff[1024];
    struct epoll_event events[1024];
    int epollfd;
    epollfd = epoll_create1(EPOLL_CLOEXEC);
    while (true) {
        //int ret = epoll_wait(epollfd, events, 1024, -1);
        connectfd = accept4(socketfd, (struct sockaddr*)nullptr, nullptr, SOCK_NONBLOCK);
        if(connectfd < 0) {
            std::cout << std::strerror(errno) << std::endl;
            continue;
        }
        recv(connectfd, buff, 1024, 0);
        std::cout << buff << std::endl;
        if(!fork()) {
            std::string str{"hello"};
            if(send(connectfd, str.c_str(), std::strlen(str.c_str()), 0) < 0) {
                std::cout << std::strerror(errno) << std::endl;
                close(connectfd);
                std::exit(EXIT_FAILURE);
            }
           close(connectfd);
        }
    }

    close(socketfd);
    return 0;
}
