#pragma once


#include <sys/epoll.h>

class EPoll
{
private:
    int epollfd; //epoll文件描述符
    struct epoll_event event;
    struct epoll_event events[512];

public:
    EPoll();
    ~EPoll();

    void AddEpoll(int socketfd);
    void DelEpoll(int socketfd);
    void EpollWait();
};
