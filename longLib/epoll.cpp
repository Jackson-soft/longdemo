#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>



int main()
{
    int socketfd,confd;
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    return 0;
}
