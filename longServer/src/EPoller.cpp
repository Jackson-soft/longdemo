#include "EPoller.h"
#include <unistd.h>


EPoller::EPoller()
{
    epfd = epoll_create1(EPOLL_CLOEXEC);
    if(epfd == -1){
        //...
    }
    event.events = EPOLLIN | EPOLLET; //边沿触发
    //const int MAXEVENTS = 512;
    //events = new epoll_event[512];
}

EPoller::~EPoller()
{
    ::close(epfd);
}

void EPoller::AddEpoll(int socketfd)
{
    epoll_ctl(epfd, EPOLL_CTL_ADD, socketfd, &event);
}


void EPoller::DelEpoll(int socketfd)
{
    epoll_ctl(epfd, EPOLL_CTL_DEL, socketfd, &event);
}

void EPoller::EpollWait()
{
    while(true) {
        //timeout：-1永久阻塞，0立即返回，非阻塞，>0指定微秒数
        int activeCnt = epoll_wait(epfd, events, 512, 0);
        if(activeCnt <= 0) {
            //这里做点啥呢
            continue;
        }
        for(int i = 0; i < activeCnt; ++i) {
            //if(events[i].data.fd == listenFd)
            if(events[i].events & EPOLLIN) {
                //read
            }else if(events[i].events & EPOLLOUT) {
                //write
            }
        }
    }
}
