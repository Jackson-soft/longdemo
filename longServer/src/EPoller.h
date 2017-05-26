#pragma once


#include <sys/epoll.h>

class EPoller
{
private:
    int epfd; //epoll文件描述符
    struct epoll_event event;
    struct epoll_event events[512];

public:
    EPoller();
    ~EPoller();

    void AddEpoll(int socketfd);
    void DelEpoll(int socketfd);
    void EpollWait();
};
