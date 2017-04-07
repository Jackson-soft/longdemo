#include "EPoll.h"
#include <unistd.h>


EPoll::EPoll()
{
    epollfd = epoll_create1(EPOLL_CLOEXEC);
    if(epollfd == -1){
        //...
    }
    event.events = EPOLLIN | EPOLLET; //边沿触发
    //const int MAXEVENTS = 512;
    //events = new epoll_event[512];
}

EPoll::~EPoll()
{
    ::close(epollfd);
}

void EPoll::AddEpoll(int socketfd)
{
    epoll_ctl(epollfd, EPOLL_CTL_ADD, socketfd, &event);
}


void EPoll::DelEpoll(int socketfd)
{
    epoll_ctl(epollfd, EPOLL_CTL_DEL, socketfd, &event);
}

void EPoll::EpollWait()
{
    int nRead = epoll_wait(epollfd, events, 512, 10);
    if(nRead < 0){

    }
}
