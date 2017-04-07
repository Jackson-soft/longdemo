#include "Socket.h"



Socket::Socket()
{
    fSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(fSocket < 0){

    }
}


Socket::~Socket()
{
    ::close(fSocket);
}
