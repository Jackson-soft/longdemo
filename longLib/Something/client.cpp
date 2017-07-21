#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>

#include <string>

int main()
{
    int socketfd;
    struct sockaddr_in servAddr;
    socketfd = socket(AF_INET, SOCK_STREAM, 0);

    std::memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(8088);
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(connect(socketfd, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0){
        std::cout << std::strerror(errno) << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::string str{"ddddddddd"};
    if(send(socketfd, str.c_str(), std::strlen(str.c_str()), 0)) {
        std::cout << std::strerror(errno) << std::endl;
        std::exit(EXIT_FAILURE);
    }

    while (true) {
        //recv();
    }
    close(socketfd);
    return 0;
}
